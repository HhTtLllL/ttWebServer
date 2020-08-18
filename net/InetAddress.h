#ifndef TT_NET_INETADDRESS_H
#define TT_NET_INETADDRESS_H


#include "../base/copyable.h"
//TODO Stringpiece
#include <netinet/in.h>

#include <string>

namespace tt
{

namespace net
{

namespace socket
{
	const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
}//socket


class InetAddress : public tt::copyable
{

public:
	explicit InetAddress(uint16_t port = 0 , bool loopbackOnly = false);

	InetAddress(std::string ip, uint16_t port);

	//仅指定 port ，不指定 ip 则ip 为INADDR_ANY 0.0.0.0
	
	explicit InetAddress(const struct sockaddr_in& addr)
		:m_addr(addr){
		
		
	}

	sa_family_t family() const { return m_addr.sin_family; }

	std::string toIp() const;
	std::string toIpPort() const;
	
	uint16_t toPort() const;

  
	const struct sockaddr* getSockAddr() const { return socket::sockaddr_cast(&m_addr); }
  
	void setSockAddrInet(const struct sockaddr_in& addr4) { m_addr = addr4; }

	uint32_t ipNetEndian() const;
	uint16_t portNetEndian() const { return m_addr.sin_port; }

	static bool resolve(std::string hostname, InetAddress* result);

private:
	struct sockaddr_in m_addr;  //网际地址

};


}//net


}// tt





#endif
