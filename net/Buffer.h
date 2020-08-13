#ifndef TT_NET_BUFFER_H
#define TT_NET_BUFFER_H


#include "../base/copyable.h"
#include "../base/Common.h"
#include "Endian.h"
#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>
#include <string>


namespace tt
{

namespace net
{

/*
      设计要点:
        1.  对外表现为一块连续的内存(char* ,len ),一遍方便客户代码的编写
        2.  其size() 可以自动增长,以适应不同大小的消息. 它不是一个 定长数组
        3. 内部以 vector of char 来保存数据,并提供相应的访问函数
*/


class Buffer : public tt::copyable{

public:
	static const size_t kCheapPrepend = 8; //预留空间
	static const size_t kInitialSize = 1024; //初始空间

	explicit Buffer(size_t initialSize = kInitialSize)
		:m_buffer(kCheapPrepend + kInitialSize),
		m_readindex(kCheapPrepend),
		m_writeindex(kCheapPrepend){

			assert(readableBytes() == 0);
			assert(writeableBytes() == initialSize);
			assert(prependableBytes() == kCheapPrepend);
	}


	void swap(Buffer& rhs){
	
		m_buffer.swap(rhs.m_buffer);
		std::swap(m_readindex, rhs.m_readindex);
		std::swap(m_writeindex, rhs.m_writeindex);
	}

	size_t readableBytes() const { return m_writeindex - m_readindex; }
	size_t writeableBytes() const { return m_buffer.size() - m_writeindex; }
	size_t prependableBytes() const { return m_readindex; }

	//返回读指针
	const char* peek() const { return begin() + m_readindex; }

	// 查找  \r\n   kCRLF  == \r\n
	// 从peek 返回的指针开始查
	const char* findCRLF() const{
	
		const char* crlf = std::search(peek(), beginWrite(),kCRLF,  kCRLF + 2);

		return crlf == beginWrite() ? NULL : crlf;
	}

	//从start 开始查找
	const char* findCRLF(const char* start) const {
	
		assert(peek() <= start);
		assert(start <= beginWrite());

		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);

		return crlf == beginWrite() ? NULL : crlf;
	}


	//从 peek() 开始查找 \n
	const char* findEOL() const{
	
		const void* eol = memchr(peek(), '\n', readableBytes());

		return static_cast<const char*>(eol);
	}
	const char* findEOL(const char* start) const{

		assert(peek() <= start);
		assert(start <= beginWrite());
	
		const void* eol = memchr(start, '\n', beginWrite() - start);

		return static_cast<const char*>(eol);
	}


	//取回数据
	void retrieve(size_t len){
	
		assert(len <= readableBytes());

		if(len < readableBytes()){
			m_readindex += len;
		}else{
		
			//取回所有数据,调整下标
			retrieveAll();
		}
	}

	//取回数据直到 end
	
	void retrieveUntil(const char* end){
		
		assert(peek() <= end);
		assert(end <= beginWrite());
		retrieve( end - peek());
	}

	//取回8 个字节
	void retrieveInt64() { return retrieve(sizeof(int64_t)); }
	
	//取回4 个字节
	void retrieveInt32() { return retrieve(sizeof(int32_t)); }
	
	//取回2 个字节
	void retrieveInt16() { return retrieve(sizeof(int16_t)); }

	//取回1 个字节
	void retrieveInt8() { return retrieve(sizeof(int8_t)); }


	//取回所有数据
	void retrieveAll() {
	
		m_readindex = kCheapPrepend;
		m_writeindex = kCheapPrepend;
	}


	//取回所有数据返回字符串
	std::string retrieveAllAsString() { return retrieveAsString(readableBytes()); }

	//取回那一部分数据，返回字符串
	std::string retrieveAsString(size_t len) {

		assert(len <= readableBytes());
		std::string result(peek(), len);

		retrieve(len);
		return result;
	}

	//添加数据
	void append(const std::string& str) { append(str.data(),str.size()); }
	
	void append(const char* data, size_t len){
	
		//先看是否能够写下
		ensureWritableBytes(len);
		std::copy(data, data + len, beginWrite());
		hasWritten(len);
	}

	void append( const void* data, size_t len){
	
		append(static_cast<const char*>(data), len);
	}

	void ensureWritableBytes(size_t len){
	
		if(writeableBytes() < len){
		
			makeSpace(len);
		}else{
		
			assert(writeableBytes() >= len);
		}
	}

	char* beginWrite() { return begin() + m_writeindex; }
	const char* beginWrite() const { return begin() + m_writeindex; }

	void hasWritten(size_t len){
	
		assert(len <= writeableBytes());
		m_writeindex += len;
	}

	void unwrite(size_t len){
	
		assert(len <= readableBytes());
		m_writeindex -= len;
	}


	void appendInt64(int64_t x){
	
		int64_t be64 = socket::hostToNetwork64(x);
		append(&be64, sizeof(be64));
	}

	void appendInt32(int32_t x){
	
		int32_t be32 = socket::hostToNetwork32(x);
		append(&be32, sizeof(be32));
	}

	void appendInt16(int16_t x){
	
		int16_t be16 = socket::hostToNetwork16(x);
		append(&be16, sizeof(be16));
	}

	void appendInt8(int8_t x){
		
		append(&x, sizeof(x));
	}


	//读取 8 个字节
	
	int64_t readInt64(){
	
		int64_t result = peekInt64();
		retrieveInt64();

		return result;
	}
	
	int32_t readInt32(){
	
		int32_t result = peekInt32();
		retrieveInt32();

		return result;
	}

	
	int16_t readInt16(){
	
		int16_t result = peekInt16();
		retrieveInt16();

		return result;
	}

	
	int8_t readInt8(){
	
		int8_t result = peekInt8();
		retrieveInt8();

		return result;
	}

	int64_t peekInt64() const{
	
		assert(readableBytes() >= sizeof(int64_t));
		int64_t be64 = 0;

		::memcpy(&be64, peek(), sizeof(be64));

		return socket::networkToHost64(be64);
	}
	
	int32_t peekInt32() const{
	
		assert(readableBytes() >= sizeof(int32_t));
		int32_t be32 = 0;

		::memcpy(&be32, peek(), sizeof(be32));

		return socket::networkToHost64(be32);
	}


	int16_t peekInt16() const{
	
		assert(readableBytes() >= sizeof(int16_t));
		int16_t be16 = 0;

		::memcpy(&be16, peek(), sizeof(be16));

		return socket::networkToHost64(be16);
	}


	int8_t peekInt8() const{
	
		assert(readableBytes() >= sizeof(int8_t));

		int8_t x = *peek();

		return x;
	}


	//在prependInt 添加 8 个字节
	void prependInt64(int64_t x){

		int64_t be64 = socket::hostToNetwork64(x);

		prepend(&be64, sizeof(be64));
	}

	void prependInt32(int32_t x){

		int32_t be32 = socket::hostToNetwork32(x);

		prepend(&be32, sizeof(be32));
	}

	void prependInt16(int16_t x){

		int16_t be16 = socket::hostToNetwork16(x);

		prepend(&be16, sizeof(be16));
	}

	void prependInt8(int8_t x){

		prepend(&x, sizeof(x));
	}



	void prepend(const void* data, size_t len){
	
		assert(len <= prependableBytes());
		m_readindex -= len;
		const char* d = static_cast<const char*>(data);
		std::copy(d, d + len, begin() + m_readindex);
	}

	//伸缩空间， 保留 reserve 个字节
	void shrink(size_t reserve){
	
		//TODO   // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
			
		Buffer other;
		other.ensureWritableBytes(readableBytes() + reserve);
		other.append(std::string(peek(), static_cast<int>(readableBytes())));

		swap(other);
	}


	size_t internalCapacity() const {
	
		return m_buffer.capacity();
	}


	//从套接字读取数据，再将数据添加到缓冲区中
	ssize_t readFd(int fd, int* savedError);

private:

	//TODO 思考 这里先取* 然后 取地址
	char* begin() { return &*m_buffer.begin(); }

	const char* begin() const { return &*m_buffer.begin(); }

	void makeSpace(size_t len){

		if(writeableBytes() + prependableBytes() < len + kCheapPrepend){
			
			//加上 前面预留的空间如果能够写下的话, 将数据搬移到前面,在写
			// FIXME: move readable dat
			m_buffer.resize(m_writeindex + len);
		}else{
			//否则扩充空间
			// move readable data to the front, make space inside buffer
			
			assert(kCheapPrepend < m_readindex);
			size_t readable = readableBytes();

			std::copy(begin() + m_readindex, begin() + m_writeindex, begin() + kCheapPrepend);

			m_readindex = kCheapPrepend;
			m_writeindex = m_readindex + readable;

			assert(readable == readableBytes());
		}
	}





	std::vector<char> m_buffer;  //用于替代固定大小的数组
	size_t m_readindex; //读位置
	size_t m_writeindex; //写位置

	static const char kCRLF[];



};


} //net

}//tt


#endif //TT_NET_BUFFER_H
