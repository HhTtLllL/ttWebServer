#include "Buffer.h"
#include "SocketsOps.h"
#include <errno.h>
#include <sys/uio.h>
#include "../base/Common.h"

using namespace tt;
using namespace tt::net;

const char Buffer::kCRLF[] = "\r\n";
const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;


// 从 fd 读取数据, 一次尽可能多的读取数据,将当前缓冲区的数据都读取
// saved an ioctl()/FIONREAD call to tell how much to read
// 节省一次 ioctl 系统调用(获取有多少可读数据)
ssize_t Buffer::readFd(int fd, int* saveerrno){

	char extrabuf[65536]; //用来保证数据都被读取
	struct iovec vec[2];

	const size_t writable = writeableBytes();

	//第一块缓冲区
	
	vec[0].iov_base = begin() + m_writeindex;
	vec[0].iov_len = writable;

	//第二块缓冲区
	
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof(extrabuf);

	const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;

	const ssize_t n = socket::readv(fd, vec, iovcnt); //先接收数据

	//如果第一块缓冲区足够容纳
	if(implicit_cast<size_t>(n) <= writable) { m_writeindex += n; }
	else { //当前缓冲区不够容纳，因而数据都被接受到了 第二块缓冲区 extrabuf, 将其append 至 buffer

		m_writeindex = m_buffer.size();

		append(extrabuf, n - writable);
	}

	return n;
}
