#ifndef TT_NET_TCPCONNECTION_H
#define TT_NET_TCPCONNECTION_H

#include "../base/noncopyable.h"
#include "../base/Common.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "InetAddress.h"
#include <memory>
#include <boost/any.hpp>
#include <string>

struct tcp_info;

namespace tt
{

namespace net
{

class Channel;
class EventLoop;
class Socket;

class TcpConnection : noncopyable, public  std::enable_shared_from_this<TcpConnection>{

public:
	TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr);
	~TcpConnection();

	EventLoop* getLoop() const { return m_loop; }
	const std::string& name() const { return m_name; }
	const InetAddress& localAddress() const { return m_localAddr; }
	const InetAddress& peerAddress() const { return m_peerAddr; }

	bool connected() const { return m_state == kConnected; }
	bool disconnectd() const { return m_state == kDisconnected; }


	bool getTcpInfo(struct tcp_info* ) const;
	std::string getTcpInfoString() const;

	// void send(string&& message); // C++11
	void send(const void* message, int len);
	void send(const std::string& message);
	void send(Buffer* message);  
 
	void shutdown(); // 
  
  
	void forceClose();
	void setTcpNoDelay(bool on);
	
	void startRead();
	void stopRead();
	bool isReading() const { return m_reading; };

	void setContext(const boost::any& context) { m_context = context; }

	const boost::any& getContext() const { return m_context; }

	boost::any* getMutableContext() { return &m_context; }

	void setConnectionCallback(const ConnectionCallback& cb){ m_connectionCallback = cb; }
	void setMessageCallback(const MessageCallback& cb) { m_messageCallback = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb){ m_writeCompleteCallback = cb; }
	void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
	{ m_highWaterMarkCallback = cb; m_highWaterMark = highWaterMark; }

	Buffer* inputBuffer(){ return &m_inputBuffer; }
	Buffer* outputBuffer(){ return &m_outputBuffer; }

	void setCloseCallback(const CloseCallback& cb){ m_closeCallback = cb; }

	void connectEstablished(); 
	void connectDestroyed();  

private:

	enum StateE{ kDisconnected, kConnecting, kConnected, kDisconnecting };

	void handleRead();
	void handleWrite();
	void handleClose();
	//channel 中可能有错误事件
	void handleError();

	void sendInLoop(const std::string& message);
	void sendInLoop(const void * message, size_t len);
	void shutdownInLoop();


	void forceCloseInLoop();
	void setState(StateE s) { m_state = s; }

	const char* stateToString() const;

	void startReadInLoop();
	void stopReadInLoop();

	EventLoop* m_loop;
	const std::string m_name; //连接名
	StateE m_state;
	bool m_reading;

	std::unique_ptr<Socket> m_socket;
	std::unique_ptr<Channel> m_channel;			//channel 关注socket 可读可写等事件


	const InetAddress m_localAddr; //本地地址
	const InetAddress m_peerAddr; //对端地址
	ConnectionCallback m_connectionCallback;
	MessageCallback m_messageCallback;

	//大流量的需要关注
	/*
	   不断生成数据,然后发送conn->send()
	   如果对等方接收不及时,受到通告窗口的控制,内核发送缓冲不足,这个时候,就会将用户数据添加到
	   应用层发送缓冲区, 可能会撑爆 应用层的发送缓冲区
	   解决方法就是调整发送频率,  ----关注 writecompleCallback
	   所有数据都发送完, writecompleCallback  回调,然后继续发送
	*/

	WriteCompleteCallback m_writeCompleteCallback;
	HighWaterMarkCallback m_highWaterMarkCallback;
	CloseCallback m_closeCallback;

	size_t m_highWaterMark;  //高水位标

	Buffer m_inputBuffer;
	Buffer m_outputBuffer;

	boost::any m_context; 
};


typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}//net

}//tt

#endif //TT_NET_TCPCONNECTION_H