#ifndef TT_NET_TCPSERVER_H
#define TT_NET_TCPSERVER_H

#include "../base/Common.h"
#include "TcpConnection.h"
#include "../base/Atomic.h"

#include <map>

namespace tt
{

namespace net
{
	
class Acceptor;
class EventLoop;
class EventLoopThreadPool;


class TcpServer : noncopyable{


public:
	typedef std::function<void(EventLoop*)> ThreadInitCallback;

	enum Option{

		kNoReusePort,
		kReusePort,
	};



	TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, Option option = kReusePort);
	~TcpServer();

	const std::string& ipPort() const { return m_ipPort; }
	const std::string& name() const { return m_name; }
	EventLoop* getLoop() const { return m_loop; }

	void setThreadNum(int numThreads);

	void setThreadInitCallback( const ThreadInitCallback& cb) { m_threadInitCallback = cb; }


	std::shared_ptr<EventLoopThreadPool> threadPool() { return m_threadPool; }

	void start();

	void setConnectionCallback( const ConnectionCallback& cb) { m_connectionCallback = cb; }
	void setMessageCallback(const MessageCallback& cb) { m_messageCallback = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { m_writeCompleteCallback = cb; }





private:

	void newConnection(int sockfd, const InetAddress& peerAddr);
	void removeConnection(const TcpConnectionPtr& conn);
	void removeConnectionInLoop(const TcpConnectionPtr& conn);

	typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

	EventLoop* m_loop;
	const std::string m_ipPort; //服务端口
	const std::string m_name; 

	std::unique_ptr<Acceptor> m_acceptor;
	std::shared_ptr<EventLoopThreadPool> m_threadPool;


	ConnectionCallback m_connectionCallback;
	MessageCallback m_messageCallback;

	WriteCompleteCallback m_writeCompleteCallback;
	ThreadInitCallback m_threadInitCallback;

	AtomicInt32 m_started;

	int m_nextConnId;

	ConnectionMap m_connections;
};

}//net

}//tt













#endif //TT_NET_TCPSERVER_H
