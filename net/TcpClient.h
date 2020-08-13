#ifndef TT_NET_TCPCLIENT_H
#define TT_NET_TCPCLIENT_H


#include "../base/MutexLock.h"
#include "TcpConnection.h"
#include "../base/Common.h"





namespace tt
{

namespace net
{

class Connector;

typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable{


public:

	TcpClient(EventLoop* loop, const InetAddress& serverAddr, const std::string& name);
	~TcpClient();

	void connect();
	void disconnect();
	void stop();
	TcpConnectionPtr connection() const{
	
		MutexLockGuard lock(m_mutex);

		return m_connection;
	}

	EventLoop* getLoop() const { return m_loop; }

	bool retry() const { return m_retry; }

	void enableRetry() { m_retry = true;}

	const std::string& name() const{ return m_name; }

	void setConnectionCallback(ConnectionCallback cb) { m_connectionCallback = std::move(cb); }
	void setMessageCallback(MessageCallback cb) { m_messageCallback = std::move(cb); }
	void setWriteCompleteCallback(WriteCompleteCallback cb) { m_writeCompleteCallback = std::move(cb); }

private:

	void newConnection(int sockfd);
	void removeConnection(const TcpConnectionPtr& conn);

	EventLoop* m_loop;
	ConnectorPtr m_connector;
	const std::string m_name;

	ConnectionCallback m_connectionCallback;
	MessageCallback m_messageCallback;
	WriteCompleteCallback m_writeCompleteCallback;

	bool m_retry;
	bool m_connect;

	int m_nextConnId;  // name + nextConnId 表示一个连接
	mutable MutexLock m_mutex;

	//TcpConnectionPtr m_connection (m_mutex);
	
	TcpConnectionPtr m_connection THREAD_ANNOTATION_ATTRIBUTE__(guatded_by(m_mutex));


};


}//net




}//tt





















#endif //TT_NET_TCPCLIENT_H
