#include "TcpServer.h"
#include "../base/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "SocketsOps.h"
#include <iostream>

#include <stdio.h>

using namespace tt;
using namespace tt::net;


//TODO  m_loop(CHECK_NOTNULL(loop))

bool check_notNULL(EventLoop* loop)
{
	if(loop == NULL) {
	
		LOG << "TcpServr loop == null";
		abort();
	}

	return loop;
}

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, Option option)
	: m_loop(loop),  //CHECK -- 检查 loop 不是一个空指针
	m_ipPort(listenAddr.toIpPort()),  //端口号
	m_name(name),
	m_acceptor(new Acceptor(loop, listenAddr, option == kReusePort)),
	m_threadPool(new EventLoopThreadPool(loop, m_name)),
	m_connectionCallback(defaultConnectionCallback),
	m_messageCallback(defaultMessageCallback),
	m_nextConnId(1){

		std::cout << "port = " << m_ipPort << std::endl;
	
		m_acceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer(){

	m_loop->assertInLoopThread();

	LOG << "~TcpServer";

	for(auto& item : m_connections){
	
		TcpConnectionPtr conn(item.second);
		item.second.reset();
		conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
	}
}

void TcpServer::setThreadNum(int numThreads){


	assert(0 <= numThreads);
	m_threadPool->setThreadNum(numThreads);
}

void TcpServer::start(){

	if(m_started.getAndSet(1) == 0){
	

		m_threadPool->start(m_threadInitCallback);

		//判断是否处于监听状态

		assert(!m_acceptor->listenning());

		m_loop->runInLoop(std::bind(&Acceptor::listen, get_pointer(m_acceptor)));
	}
}

 
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr){
	std::cout << "  new Connection " << std::endl;
	m_loop->assertInLoopThread();

	EventLoop* ioLoop = m_threadPool->getNextLoop();

	char buf[64];
	snprintf(buf, sizeof(buf), "-%s#%d", m_ipPort.c_str(), m_nextConnId);

	std::cout <<"m_name = " << m_name << "buf = " <<  buf << std::endl;
	++m_nextConnId;

  // m_name  从 httpServer 中传来
	std::string connName = m_name + buf;

	//LOG << "new Connection ";

	InetAddress localAddr(socket::getLocalAddr(sockfd));
	TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));

	m_connections[connName] = conn;

	conn->setConnectionCallback(m_connectionCallback);
	conn->setMessageCallback(m_messageCallback);
	conn->setWriteCompleteCallback(m_writeCompleteCallback); 

	conn->setCloseCallback(std::bind(&TcpServer::removeConnection,this,_1));

	ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}


void TcpServer::removeConnection(const TcpConnectionPtr& conn){

	m_loop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));

}



void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn){

	m_loop->assertInLoopThread();

	//LOG << " removeConnectionInLoop";

	size_t n = m_connections.erase(conn->name());

	//TODO 为什么
	(void)n;
	assert(n == 1);

	assert(n == 1);

	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}



































