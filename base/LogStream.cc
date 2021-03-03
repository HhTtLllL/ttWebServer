#include "LogStream.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <limits>


using namespace tt;
using namespace tt::detail;
namespace tt{

namespace detail{

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

//将数值转换诶字符串
template<typename T>
size_t convert(char buf[],T value){ 

	T i = value;
	char* p = buf;

	do{

		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	}while(i != 0);

	if(value << 0){

		*p++ = '-';
	}

	*p = '\0';

	std::reverse(buf,p);

	return p - buf;
}	

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

}//detail

}//tt

//将数字编程字符串存入
template<typename T>
void LogStream::formatInteger(T v){
		
	//如果剩余空间大于 kMaxNumbericSize 字节
	if(m_buffer.avail() >= kMaxNumbericSize){ 

		size_t len = convert(m_buffer.current(), v);
		m_buffer.add(len);
	}
}

LogStream& LogStream::operator<<(short v){
	
	*this << static_cast<int>(v);

	return *this;
}

LogStream& LogStream::operator<< (unsigned short v){
	
	*this << static_cast<unsigned int>(v);

	return *this;
}

LogStream& LogStream::operator<< (int v){
	
	formatInteger(v);

	return *this;
}

LogStream& LogStream::operator<< (int unsigned v){
	
	formatInteger(v);

	return *this;
}

LogStream& LogStream::operator<< (long v){
	
	formatInteger(v);

	return *this;
}


LogStream& LogStream::operator<< (unsigned long v){
	
	formatInteger(v);

	return *this;
}

LogStream& LogStream::operator<< (long long v){
	
	formatInteger(v);

	return *this;
}

LogStream& LogStream::operator<< (unsigned long long v){
	
	formatInteger(v);

	return *this;
}

LogStream& LogStream::operator<< (double v){
	
	if(m_buffer.avail() >= kMaxNumbericSize){

		int len = snprintf(m_buffer.current(),kMaxNumbericSize,"%.12g",v);

		m_buffer.add(len);
	}

	return *this;
}

LogStream& LogStream::operator<< (long double v){
	
	if(m_buffer.avail() >= kMaxNumbericSize){

		int len = snprintf(m_buffer.current(),kMaxNumbericSize,"%.12Lg",v);

		m_buffer.add(len);
	}

	return *this;
}