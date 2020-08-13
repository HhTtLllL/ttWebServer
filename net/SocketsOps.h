#ifndef TT_NET_SOCKETSOPS_H
#define TT_NET_SOCKETSOPS_H

#include <arpa/inet.h>
#include "../base/Common.h"

namespace tt
{

namespace net
{


namespace socket
{

//对socket 相关系统调用进行封装,全局函

	int createNonblockingOrDie(sa_family_t family);

	int connect(int sockfd, const struct sockaddr* addr);
	void bindOrdie(int sockfd, const struct sockaddr* addr);
	void listenOrdie(int sockfd);
	int accept(int sockfd, struct sockaddr_in* addr);

	ssize_t read(int sockfd, void *buf, size_t count);
	ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);
	ssize_t write(int sockfd, const void* buf, size_t count);
	//TODO writev 待实现
	
	void close(int sockfd);
	void shutdownWrite(int sockfd);


	void toIpPort(char* buf, size_t size, const struct sockaddr* addr);
	void toIp(char* buf, size_t size, const struct sockaddr* addr);

	void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

	int getSocketError(int sockfd);

	const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
	struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);
	const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);


	struct sockaddr_in getLocalAddr(int sockfd);
	struct sockaddr_in getPeerAddr(int sockfd);
	bool isSelfConnect(int sockfd);
}//socket

}//net

}//tt





#endif
