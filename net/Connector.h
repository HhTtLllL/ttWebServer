#ifndef TT_NET_CONNECTOR_H
#define TT_NET_CONNECTOR_H



#include "../base/noncopyable.h"
#include "InetAddress.h"
#include <functional>
#include <memory>

namespace tt
{

namespace net
{

class Channel;
class EventLoop;

//主动连接的抽象
//主动发起连接,带有自动重连的功能

class Connector : noncopyable, public std::enable_shared_from_this<Connector>{

public:
	typedef std::function<void(int sockfd)> NewConnectionCallback;
	Connector(EventLoop* loop, const InetAddress& serverAddr);
	~Connector();

	void setNewConnectionCallback(const NewConnectionCallback& cb) { m_newConnectionCallback = cb; }

	void start();
	void restart();
	void stop();

	const InetAddress& serverAddress() const { return m_serverAddr; }





private:


	enum States{ kDisconnected, kConnecting, kConnected };

	static const int kMaxRetryDelayMs = 30*1000; //30 秒最大重连延迟时间
	static const int kInitRetryDelayMs = 500; // 0.5 初始状态连接不上，0.5 秒后重连‘


	void setState(States s) { m_state = s; }
	void startInLoop();
	void stopInLoop();
	void connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd) ;
	int removeAndResetChannel();
	void resetChannel();



	EventLoop* m_loop; 
	InetAddress m_serverAddr;
	bool m_connect; 
	States m_state;
	std::unique_ptr<Channel> m_channel;  
	NewConnectionCallback m_newConnectionCallback;  //连接成功回调函数
	int m_retryDelayMs;  //重连延迟时间








};



}// net

}//tt









#endif // TT_NET_CONNECTOR_H
