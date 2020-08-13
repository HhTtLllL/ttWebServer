#ifndef TT_NET_SOCKET_H
#define TT_NET_SOCKET_H

#include "../base/noncopyable.h"

struct tcp_info;

namespace tt
{

namespace net
{

class InetAddress;


class Socket : noncopyable{

	
public:

	explicit Socket(int sockfd)
		:m_sockfd(sockfd){

	}

	~Socket();

	int fd() const { return m_sockfd; }
	bool getTcpInfo(struct tcp_info*) const;

	bool getTcpInfoString(char* buf, int len) const;

	void bindAddress(const InetAddress& localaddr);

	void listen();

	int accept(InetAddress* peeraddr);
	void shutdownWrite();

	void setTcpNoDelay(bool on);

	//设置地址复用
	void setReuseAddr(bool on);
	//设置端口复用
	void setReusePort(bool on) ;

	//TCP keepalive 是只定期探测连接是否存在,如果应用层有心跳的话,这个选项不是必须要设置的
	void setKeepAlive(bool on);



private:
	const int m_sockfd;




};



} //net 


}//tt 







#endif // TT_NET_SOCKET_H
