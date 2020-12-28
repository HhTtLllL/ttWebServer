#include "Socket.h"
#include "../base/Logging.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>


using namespace tt;
using namespace tt::net;

Socket::~Socket(){

	socket::close(m_sockfd);
}

bool Socket::getTcpInfo(struct tcp_info* tcpi) const{

	socklen_t len = sizeof(*tcpi);
	memset(tcpi, 0, len);

	//将获得的信息传入到 tcpi 中
	return ::getsockopt(m_sockfd, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

void Socket::bindAddress(const InetAddress& addr){

	socket::bindOrdie(m_sockfd, addr.getSockAddr());
}

void Socket::listen(){

	socket::listenOrdie(m_sockfd);
}

int Socket::accept(InetAddress* peeraddr){

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	int connfd = socket::accept(m_sockfd, &addr);
	if(connfd >= 0){ peeraddr->setSockAddrInet(addr); }

	return connfd;
}

void Socket::shutdownWrite(){

	socket::shutdownWrite(m_sockfd);
}

//禁用 negly 算法
void Socket::setTcpNoDelay(bool on){


	int optval = on ? 1 : 0;
	::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval)));
}


void Socket::setReuseAddr(bool on){

	int optval = on ? 1 : 0;

	::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}


void Socket::setReusePort(bool on){

	int optval = on ? 1 : 0;

	int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval)));
}


void Socket::setKeepAlive(bool on){


	int optval = on ? 1 : 0;

	::setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval)));

}