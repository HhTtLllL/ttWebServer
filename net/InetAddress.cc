#include "InetAddress.h"
#include "../base/Logging.h"
#include "Endian.h"
#include "SocketsOps.h"

#include <netdb.h>
#include <netinet/in.h>

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;


using namespace tt;
using namespace tt::net;


static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");


InetAddress::InetAddress(uint16_t port, bool loopbackOnly){

	static_assert(offsetof(InetAddress,m_addr) == 0, "addr_offset 0 ");

	memset(&m_addr,0,sizeof(m_addr));
	m_addr.sin_family = AF_INET;

	in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;

	m_addr.sin_addr.s_addr = socket::hostToNetwork32(ip);
	m_addr.sin_port = socket::hostToNetwork16(port);
}



InetAddress::InetAddress(std::string ip, uint16_t port){

	memset(&m_addr,0,sizeof(m_addr));
	socket::fromIpPort(ip.c_str(), port, &m_addr);

}


std::string InetAddress::toIpPort() const{

	char buf[64] = "";
	socket::toIpPort(buf, sizeof(buf), getSockAddr());

	return buf;
}




uint32_t InetAddress::ipNetEndian() const{

	assert(family() == AF_INET);
	return m_addr.sin_addr.s_addr;
}

uint16_t InetAddress::toPort() const{
  
	return socket::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64 * 1024];

//TODO resolve
/*
bool InetAddress::resolve(std::string hostname, InetAddress* out){

	assert(out != NULL);



}
*/






































































