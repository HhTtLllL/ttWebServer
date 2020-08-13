#include "TcpClient.h"
#include "../base/Logging.h"
#include "Connector.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <stdio.h>


using namespace tt;
using namespace tt::net;


namespace tt
{

namespace net
{

namespace detail
{

	void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn){
	
		loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
	}

	//TODO
	void removeConnector(const ConnectorPtr& connector){
	
	}
}//detail
}//net
};//tt



TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr, const std::string& name) 
	:m_loop(loop),
	m_connector(new Connector(loop,serverAddr)),
	m_name(name),
	m_connectionCallback(defaultConnectionCallback),
	m_messageCallback(defaultMessageCallback),
	m_retry(false),
	m_connect(true),
	m_nextConnId(1){

		//设置连接好 的回调函数
		m_connector->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this,_1));

		LOG << "TcpClient::TcpClient";
	
}

TcpClient::~TcpClient(){

	TcpConnectionPtr conn;
	bool unique = false;

	{
		MutexLockGuard lock(m_mutex);
		unique = m_connection.unique();
		conn = m_connection;
	}

	if(conn){
	
		assert(m_loop == conn->getLoop());

		CloseCallback cb = std::bind(&detail::removeConnection, m_loop, _1);
		m_loop->runInLoop(std::bind(&TcpConnection::setCloseCallback, conn, cb));

		if(unique) conn->forceClose();
		else {
		
			m_connector->stop();
			//TODO
		}
	}
}


void TcpClient::connect(){

	LOG << "TcpClient::connect";

	m_connect = true;
	m_connector->start();  //发起连接
}


void TcpClient::disconnect(){

	m_connect = false;

	{
		MutexLockGuard lock(m_mutex);
		if(m_connection) m_connection->shutdown();
	}
}


void TcpClient::stop(){

	m_connect = false;
	m_connector->stop();
}


void TcpClient::newConnection(int sockfd){

	m_loop->assertInLoopThread();
	InetAddress peerAddr(socket::getPeerAddr(sockfd));

	char buf[32];
	
	snprintf(buf, sizeof(buf), "%s#%d", peerAddr.toIpPort().c_str(), m_nextConnId);

	++m_nextConnId;

	std::string connName = m_name + buf;
	InetAddress localAddr(socket::getLocalAddr(sockfd));



	TcpConnectionPtr conn(new TcpConnection(m_loop, connName, sockfd, localAddr, peerAddr));

	conn->setConnectionCallback(m_connectionCallback);
	conn->setMessageCallback(m_messageCallback);
	conn->setWriteCompleteCallback(m_writeCompleteCallback);
	conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1));


	{
		MutexLockGuard lock(m_mutex);
		m_connection = conn;
	}


	conn->connectEstablished();   //这里回调connectionCallBack_   就是关注事件的可读事
}


void TcpClient::removeConnection(const TcpConnectionPtr& conn){

	m_loop->assertInLoopThread();
	assert(m_loop == conn->getLoop());

	{
	
		MutexLockGuard lock(m_mutex);
		assert(m_connection == conn);

		m_connection.reset();
	}

	m_loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
	if(m_retry && m_connect){
	
		LOG << "TcpClient::removeConnection  重连";

		m_connector->restart();
	}
}


































