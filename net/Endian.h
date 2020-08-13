#ifndef TT_NET_ENDIAN_H
#define TT_NET_ENDIAN_H


//封装 字节序转换函数
#include <stdint.h>
#include <endian.h>


namespace tt
{

namespace net
{

namespace socket
{
	//将主机字节序 转换为 大端字节序(网络字节序)
	// 注意: 这个函数 不可移植 (没在posix标准中 中)
	inline uint64_t hostToNetwork64(uint64_t host64){
  
		return htobe64(host64);
	}

	inline uint32_t hostToNetwork32(uint32_t host32){

		return htobe32(host32);
	}

	inline uint16_t hostToNetwork16(uint16_t host16){

		return htobe16(host16);
	}

	/*inline uint8_t hostToNetwork8(uint8_t host8){

		return htobe8(host8);
	}*/

	//网络字节序转化为 主机字节序
	inline uint64_t networkToHost64(uint64_t net64){

		return be64toh(net64);
	}

	inline uint32_t networkToHost32(uint32_t net32){

		return be32toh(net32);
	}

	inline uint16_t networkToHost16(uint16_t net16){

		return be16toh(net16);
	}








} //socket





}// net


}// t













#endif
