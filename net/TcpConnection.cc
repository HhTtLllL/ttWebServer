#include "TcpConnection.h"
#include "../base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"
#include <error.h>
#include <iostream>


using namespace tt;
using namespace tt::net;


void tt::net::defaultConnectionCallback(const TcpConnectionPtr& conn){

	//TODO

}

void tt::net::defaultMessageCallback(const TcpConnectionPtr&, Buffer* buf){

	buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr)
	:m_loop(loop),
	m_name(name),
	m_state(kConnecting),
	m_reading(true),
	m_socket(new Socket(sockfd)), //构建一个套接字
	m_channel(new Channel(loop,sockfd)),
	m_localAddr(localAddr),
	m_peerAddr(peerAddr),
	m_highWaterMark(64*1024*1024){

									//TODO 这里的setReadCallback 少了一个时间戳 _1
		m_channel->setReadCallback(std::bind(&TcpConnection::handleRead, this)); 
		m_channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
		m_channel->setCloseCallBack(std::bind(&TcpConnection::handleClose,this));
		m_channel->setErrorCallBack(std::bind(&TcpConnection::handleError, this));

		//LOG << " init TcpConnection::TcpConnection";

		m_socket->setKeepAlive(true);
}

TcpConnection::~TcpConnection(){

	//LOG << "TcpConnection::~TcpConnection";
	assert(m_state == kDisconnected);
}

bool TcpConnection::getTcpInfo(struct tcp_info* tcpi) const{

	return m_socket->getTcpInfo(tcpi);
}

void TcpConnection::send(const void* data, int len){

	send(std::string(static_cast<const char*>(data), len));

}

//TODO 有待理解
void TcpConnection::send(const std::string& message){

	if(m_state == kConnected){
	
		if(m_loop->isInLoopThread()) sendInLoop(message);
		else {
		
			void (TcpConnection::*fp)(const std::string& message) = &TcpConnection::sendInLoop;
			m_loop->runInLoop(std::bind(fp, this, message));
		}
	}
}


void TcpConnection::send(Buffer* buf){

	if(m_state == kConnected){
		
		if(m_loop->isInLoopThread()){
		
			sendInLoop(buf->peek(), buf->readableBytes());
			buf->retrieveAll();
		}else{
			
			void (TcpConnection::*fp)(const std::string& message) = &TcpConnection::sendInLoop;
			m_loop->runInLoop(std::bind(fp, this, buf->retrieveAllAsString()));
		}
	}
}


void TcpConnection::sendInLoop(const std::string& message){

	sendInLoop(message.data(), message.size());
}


void TcpConnection::sendInLoop(const void* data, size_t len){

	m_loop->assertInLoopThread();

	ssize_t nwrote = 0;
	size_t remaining = len;
	bool faultError = false;

	if(m_state == kDisconnected){
	
		LOG << "TcpConnection::sendInLoop";
		return ;
	}

	  
	//是否关注可写事件
	//通道 没有关注可写事件  并且   发送缓冲区没有数据,直接writ
	if(!m_channel->isWriting() && m_outputBuffer.readableBytes() == 0){
	
		nwrote = socket::write(m_channel->fd(), data, len);

		if(nwrote >= 0){
		
			remaining = len - nwrote;
			if(remaining == 0 && m_writeCompleteCallback){
			
				m_loop->queueInLoop(std::bind(m_writeCompleteCallback, shared_from_this()));
			}
		}else{ // nwrote < 0 出错
		
			if(errno != EWOULDBLOCK){

				faultError = true;
			
				LOG << "TcpConnection::sendInLoop";
			}
		}
		
		

	}

	assert(remaining <= len);
	//没有错误,并且还有未写完的数据(说明内核发送缓冲区满,要将未写完的数据添加到 output buffer 中)
	if(!faultError && remaining > 0){
	
		size_t oldlen = m_outputBuffer.readableBytes();
		//如果超过highwaterMark_ ( 高水位标),回调 highWaterMarkCallback
		if(oldlen + remaining >= m_highWaterMark && oldlen < m_highWaterMark && m_highWaterMarkCallback){
		
			m_loop->queueInLoop(std::bind(m_highWaterMarkCallback, shared_from_this(), oldlen + remaining));
		}

		m_outputBuffer.append(static_cast<const char*>(data) + nwrote, remaining);

		if(!m_channel->isWriting()){
		
			m_channel->enableWriting(); //关注 EPOLLOUT 事件
		}
	}
}


void TcpConnection::shutdown(){
	  
	// FIXME: use compare and swap

	if(m_state == kConnected){
	
		setState(kDisconnecting);

		    // FIXME: shared_from_this()
		m_loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
	}

}
 /*
    服务器端主动断开与客户端的连接
    这意味和 客户端 read 返回为 0 
     客户端 调用  close(conn); 
    此时 服务器端收到 POLLHUP | POLLIN

*/
void TcpConnection::shutdownInLoop(){

	m_loop->assertInLoopThread();

	if(!m_channel->isWriting()){

		//关闭写的一端
		m_socket->shutdownWrite();
	}
}


void TcpConnection::forceClose(){

	if(m_state == kConnected || m_state == kDisconnecting){
	
		setState(kDisconnecting);
		m_loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop,shared_from_this()));
	}
}


//void TcpConnection::forceCloseWithDelay()

void TcpConnection::forceCloseInLoop(){

	m_loop->assertInLoopThread();

	if(m_state == kConnected || m_state == kDisconnecting){
	
		handleClose();
	}
}

void TcpConnection::setTcpNoDelay(bool on){

	m_socket->setTcpNoDelay(on);
}

void TcpConnection::startRead(){

	m_loop->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}


void TcpConnection::startReadInLoop(){

	m_loop->assertInLoopThread();

	if(!m_reading || !m_channel->isReading());{
	
		m_channel->enableReading();
		m_reading = true;
	}
}

void TcpConnection::stopRead(){

	m_loop->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}

void TcpConnection::stopReadInLoop(){

	m_loop->assertInLoopThread();
	if (m_reading || m_channel->isReading()){

		m_channel->disableReading();
		m_reading = false;
	}
}

//建立连接
void TcpConnection::connectEstablished(){

	m_loop->assertInLoopThread();

	//判断状态
	assert(m_state == kConnecting);

	//设置状态
	setState(kConnected);

	//将this  转换为 智能指针  shared_ptr
	m_channel->enableReading();

	m_connectionCallback(shared_from_this());
}


void TcpConnection::connectDestroyed(){

	m_loop->assertInLoopThread();

	if(m_state == kConnected){
	
		setState(kDisconnected);
		m_channel->disableAll();

		m_connectionCallback(shared_from_this());
	}

	m_channel->remove();
}

//触发 可读事件, 由 handleEvent 调

void TcpConnection::handleRead(){

	m_loop->assertInLoopThread();

	int savedErrno = 0;
	//读通道,将数据读到 缓存区中 ,然后回调 messageCallbac
	
	ssize_t n = m_inputBuffer.readFd(m_channel->fd(), &savedErrno);

	if(n > 0){
		  //把当前对象的裸指针,会把当前Tcp对象转换为 shared_ptr
		  m_messageCallback(shared_from_this(), &m_inputBuffer);

	}else if( n == 0) handleClose();
	else{
	
		handleError();
	}
}

void TcpConnection::handleWrite(){


	m_loop->assertInLoopThread();
	//如果处于 EPOLLOUT 事件
	if(m_channel->isWriting()){
	
		ssize_t n = socket::write(m_channel->fd(), m_outputBuffer.peek(), m_outputBuffer.readableBytes());

		if(n > 0){
		
			m_outputBuffer.retrieve(n);

			if(m_outputBuffer.readableBytes() == 0){ //发送缓冲区已经清空

				m_channel->disableWriting();
				if(m_writeCompleteCallback){
				
					m_loop->queueInLoop(std::bind(m_writeCompleteCallback, shared_from_this()));
				}

				if(m_state == kDisconnecting)  shutdownInLoop();
			}

		}else  LOG << "TcpConnection::handlewrite";
	}

}


//关闭连接
void TcpConnection::handleClose(){

	m_loop->assertInLoopThread();

	assert(m_state == kConnected || m_state == kDisconnecting);

	setState(kDisconnected);

	//注销所有事件
	m_channel->disableAll();

	TcpConnectionPtr gurdThis(shared_from_this());

	m_connectionCallback(gurdThis);

	m_closeCallback(gurdThis);
}


void TcpConnection::handleError(){

	int err = socket::getSocketError(m_channel->fd());

	LOG << "TcpConnection::handleError";


}



