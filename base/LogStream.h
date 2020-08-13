#pragma once

#include <assert.h>
#include <string.h>
#include <string>
#include "noncopyable.h"

namespace tt{

class AsyncLogging;

namespace detail{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000*1000;

//缓冲区
template<int SIZE>
class FixedBuffer : noncopyable
{
public:
	FixedBuffer() : m_cur(m_data) {}
	~FixedBuffer() {}

	void append(const char* buf, size_t len){
		if(avail() > static_cast<int>(len)){   //如果当前空间足够就将数据复制进去， 这里有待实现， 如果空间不够的情况
			memcpy(m_cur,buf,len);
			m_cur += len;
		}
	}

	const char* data() const { return m_data; }
	int length() const { return static_cast<int>(m_cur - m_data); }
	char* current() { return m_cur; }
	//算出当前还有多少字节
	int avail() const { return static_cast<int>(end() - m_cur); }
	void add(size_t len) { m_cur += len; }


	void reset() { m_cur = m_data; }
	void bzero() { memset(m_data,0,sizeof(m_data)); }




private:
	const char* end() const { return m_data + sizeof(m_data); }

	char m_data[SIZE];
	char* m_cur;

};

} //detail


//将字符串和数值 写入 Buffer 中
class LogStream : noncopyable{

public:
	typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;
	
	//如果是bool 类型，则输出 1 或者 0 
	LogStream& operator << (bool v){
		m_buffer.append(v ? "1" : "0",1);

		return *this;
	}

	LogStream& operator<<(short);
	LogStream& operator<<(unsigned short);
	LogStream& operator<<(int);
	LogStream& operator<<(unsigned int);
	LogStream& operator<<(long);
	LogStream& operator<<(unsigned long);
	LogStream& operator<<(long long);
	LogStream& operator<<(unsigned long long);
	
	LogStream& operator<<(const void*);
	
	LogStream& operator<<(float v){

		*this << static_cast<double>(v);

		return *this;
	}

	LogStream& operator<<(double);
	LogStream& operator<<(long double);

	LogStream& operator<<(char v){

		m_buffer.append(&v,1);

		return *this;
	}

	LogStream& operator<<(const char* str){
		if(str){
			m_buffer.append(str,strlen(str));
		}else{
			m_buffer.append("(null)",6);
		}

		return *this;
	}

	LogStream& operator<<(const unsigned char* str){
		return operator<<(reinterpret_cast<const char*>(str));
	}

	LogStream& operator<<(const std::string& v){

		m_buffer.append(v.c_str(), v.size());

		return *this;
	}

	void append(const char* data,int len){ m_buffer.append(data, len); }

	const Buffer& buffer() const { return m_buffer; }
	void resetBuffer() { m_buffer.reset(); }



private:
	void staticCheck();
	
	template<typename T>
	void formatInteger(T);
	

	Buffer m_buffer;

	static const int kMaxNumbericSize = 32;


};

} //tt
