#include "Acceptor.h"
#include "../base/Logging.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Channel.h"
#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using namespace tt;
using namespace tt::net;


Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
	:m_loop(loop),
	m_acceptSocket(socket::createNonblockingOrDie(listenAddr.family())),
	m_acceptChannel(loop, m_acceptSocket.fd()),
	m_listenning(false),
	m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC)) //预先准备一个文件描述符
{

	 assert(m_idleFd >= 0);
	 m_acceptSocket.setReuseAddr(true);
	 m_acceptSocket.setReusePort(reuseport);
	 m_acceptSocket.bindAddress(listenAddr);
	 m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this));
}


Acceptor::~Acceptor(){

	m_acceptChannel.disableAll();
	m_acceptChannel.remove();
	::close(m_idleFd);
}


void Acceptor::listen(){

	m_loop->assertInLoopThread();
	m_listenning = true;
	m_acceptSocket.listen();
	//关注套接字的 可读事件,当可读事件到来的时候, 会调用 handleRead
	m_acceptChannel.enableReading();
}

void Acceptor::handleRead(){

	m_loop->assertInLoopThread();

	InetAddress peerAddr; //准备对等方的地址
	int connfd = m_acceptSocket.accept(&peerAddr);
	if(connfd >= 0){
	
		if(m_newConnectionCallback){

			m_newConnectionCallback(connfd,peerAddr);
		}else{
		
			socket::close(connfd);
		}
	}else{
	
		if(errno == EMFILE){
		
			::close(m_idleFd);
			m_idleFd = ::accept(m_acceptSocket.fd(), NULL, NULL);

			::close(m_idleFd);
			m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC );
		}
	}
}