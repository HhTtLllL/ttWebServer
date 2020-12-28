#include "SocketsOps.h"
#include "../base/Logging.h"
#include "Endian.h"
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

//封装了socket 相关系统调用

using namespace tt;
using namespace tt::net;

namespace 
{
	typedef struct sockaddr SA; //通用地址
} //namespace 

	//将网际地址 转换为通用地址
const struct sockaddr* socket::sockaddr_cast(const struct sockaddr_in* addr){
			//先隐世转换为 const void*,然后在转为通用地址
	return static_cast<const struct sockaddr*> (implicit_cast<const void*>(addr));
}

struct sockaddr* socket::sockaddr_cast(struct sockaddr_in* addr){

	return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr_in* socket::sockaddr_in_cast(const struct sockaddr* addr){
	
	return static_cast<const struct sockaddr_in*>(implicit_cast<const void *>(addr));

}

//创建非阻塞套接字
int socket::createNonblockingOrDie(sa_family_t family){

	int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	if(sockfd < 0){
		LOG << "socket::createNonblockingOrDie";
	
	}

	return sockfd;
}

void socket::bindOrdie(int sockfd, const struct sockaddr* addr){

	int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
	if(ret < 0){
		
		LOG << "socket::bindOrdie";
	}
}

void socket::listenOrdie(int sockfd){

	//SOMAXCONN 为 128 
	int ret = ::listen(sockfd, SOMAXCONN);
	if(ret < 0){

		LOG << "socket::listenOrdie";
	}
}

int socket::accept(int sockfd, struct sockaddr_in* addr){
        socklen_t addrlen = static_cast<socklen_t>(sizeof *addr); 
	
	//socklen_t addrlen = static_cast<socklen_t>(sizeof(* addr));
	int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
	//TODO 错误处理


	return connfd;
}

/*
int socket::accept(int sockfd, struct sockadd_in* addr){
socklen_t addrlen = static_cast<socklen_t>(sizeof *addr); 
	
	//socklen_t addrlen = static_cast<socklen_t>(sizeof(* addr));
	int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);

	//TODO 错误处理

	return connfd;
}
*/
int socket::connect(int sockfd, const struct sockaddr* addr){

	return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
}


ssize_t socket::read(int sockfd, void *buf, size_t count){

	return ::read(sockfd, buf, count);
}

//read 和 readv 不同之处在于,接收的数据可以填充到多个缓冲区中
//iov  是一个数组, iovcnt 数组的个数,第一个缓冲区不够用,就将数据接收到
//第二个缓冲区
ssize_t socket::readv(int sockfd, const struct iovec *iov, int iovcnt){

	return ::readv(sockfd, iov, iovcnt);
}

ssize_t socket::write(int sockfd, const void *buf, size_t count){

	return ::write(sockfd, buf, count);
}

void socket::close(int sockfd){

	if(::close(sockfd) < 0){
	
		LOG << "socket::close";
	}
}

void socket::shutdownWrite(int sockfd){

	if(::shutdown(sockfd, SHUT_WR) < 0){
		
		LOG << "socket::shutdownWrite";
	}
}

//将地址转换为 ip  和端口的形式 ,保存在缓冲区
//ip::port
void socket::toIpPort(char* buf, size_t size, const struct sockaddr* addr){

	//将addr 中的IP放到buf中
	toIp(buf, size, addr);
	size_t end = ::strlen(buf);

	const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);

	uint16_t port = socket::networkToHost16(addr4->sin_port);

	assert(size > end);

	snprintf(buf + end, size - end, ":%u",port);
}

void socket::toIp(char* buf, size_t size, const struct sockaddr* addr){

	assert(size >= INET_ADDRSTRLEN);

	const struct sockaddr_in* addr4= sockaddr_in_cast(addr);
	::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
}

void socket::fromIpPort(const char *ip, uint16_t port, struct sockaddr_in* addr){

	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	if(::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0){
	
		LOG << "socket::fromIpPort";
	}
}

int socket::getSocketError(int sockfd){

	int optval;

	socklen_t optlen = static_cast<socklen_t>(sizeof(optval));
	/*
		int getsockopt(int socket, int level, int option_name,
            void *restrict option_value, socklen_t *restrict option_len);
 		功能：获取一个套接字的选项
	参数：
 	    socket：文件描述符
	    level：协议层次
            SOL_SOCKET 套接字层次
            IPPROTO_IP ip层次
            IPPROTO_TCP TCP层次
    option_name：选项的名称（套接字层次）
            SO_BROADCAST 是否允许发送广播信息
            SO_REUSEADDR 是否允许重复使用本地地址
           SO_SNDBUF 获取发送缓冲区长度
           SO_RCVBUF 获取接收缓冲区长度    
           SO_RCVTIMEO 获取接收超时时间
           SO_SNDTIMEO 获取发送超时时间
    option_value：获取到的选项的值
    option_len：value的长度
 返回值：
    成功：0
    失败：-1

	*/ 
	if(::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) return errno;
	else return optval;
}

//获取本地地址
struct sockaddr_in socket::getLocalAddr(int sockfd){

	struct sockaddr_in localaddr;
	memset(&localaddr, 0, sizeof(localaddr));

	socklen_t addrlen = static_cast<socklen_t>(sizeof(localaddr));

	if(::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0){
	
		LOG << "stocket::getLocalAddr";
	}

	return localaddr;
}


//获取对端地址 TODO 可以和上面的本地地址合并
struct sockaddr_in socket::getPeerAddr(int sockfd){

	struct sockaddr_in peeraddr;
	memset(&peeraddr, 0, sizeof(peeraddr));

	socklen_t addrlen = static_cast<socklen_t>(sizeof(peeraddr));

	if(::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0){
	
		LOG << "stocket::getLocalAddr";
	}

	return peeraddr;
}

//判断是否为自连接
bool socket::isSelfConnect(int sockfd){

	struct sockaddr_in localaddr = getLocalAddr(sockfd);
	struct sockaddr_in peeraddr = getPeerAddr(sockfd);
	
	if (localaddr.sin_family == AF_INET){

		const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
		const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
		
		return laddr4->sin_port == raddr4->sin_port && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
	}
	else {

		return false;
	}
}