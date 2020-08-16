#ifndef TT_NET_ACCEPTOR_H
#define TT_NET_ACCEPTOR_H

#include "Channel.h"
#include "Socket.h"
#include <functional>

namespace tt
{

namespace net
{

//被动连接的抽象， 关注监听套接字的可读事件
class EventLoop;
class InetAddress;

 /*
    主要功能是: socket ,bind ,listen

    一般来说,在上层应用程序中,我们不直接使用 Acceptor ,而是把它作为 TcoServer 的成员


    当有了新的用户连接的时候, acceptor 有一个监听套接字, 这个套接字处于可读状态 ,然后 channel  会观察这个套接字的
    可读事件,channel 处于活跃的通道, poller 将它返回 回来,调用handevent , handevent 调用 acceptor  中的 handleread,
    handleRead 调用 accept 建立连接, 并且回调 new connection  函数

*/

class Acceptor : noncopyable{

public:

	typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

	Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
	~Acceptor();


	void setNewConnectionCallback(const NewConnectionCallback& cb){ m_newConnectionCallback = cb; }

	bool listenning() const { return m_listenning; }
	void listen();

private:
	
	void handleRead();

	EventLoop* m_loop;
	Socket m_acceptSocket;
	Channel m_acceptChannel;


	//用户设置回调函数
	
	NewConnectionCallback m_newConnectionCallback;

	bool m_listenning;
	int m_idleFd;
};





} //net


}// tt



#endif //TT_NET_ACCEPTOR_H
