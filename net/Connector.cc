#include "Connector.h"
#include "../base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <errno.h>

using namespace tt;
using namespace tt::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
	:m_loop(loop),
	m_serverAddr(serverAddr),
	m_connect(false),
	m_state(kDisconnected),
	m_retryDelayMs(kInitRetryDelayMs){
		LOG << "conntec::connetc error";

}


Connector::~Connector(){

	assert(!m_channel);
}

void Connector::start(){

	m_connect = true;
	m_loop->runInLoop(std::bind(&Connector::startInLoop,this));
}


void Connector::startInLoop(){

	m_loop->assertInLoopThread();
	assert(m_state = kDisconnected);

	if(m_connect) connect();
	else LOG << "connect ERROR";
}


void Connector::stop(){

	m_connect = false;
	m_loop->queueInLoop(std::bind(&Connector::stopInLoop,this));
}


void Connector::stopInLoop(){

	m_loop->assertInLoopThread();
	if(m_state == kConnecting){
	
		setState(kDisconnected);
		int sockfd = removeAndResetChannel(); //将通道从 epoll中移除关注，并将Channel 置空

		retry(sockfd); //这里并非重连，而调用socket::close(sockfd)
	}
}


void Connector::connect(){

	int sockfd = socket::createNonblockingOrDie(m_serverAddr.family());

	int ret = socket::connect(sockfd, m_serverAddr.getSockAddr());

	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno){
		case 0:
		case EINPROGRESS:  //非阻塞套及诶子,未连接成功返回吗是 EINPROGESS 表示正在连接
		case EINTR:
		case EISCONN: // 连接成功
			connecting(sockfd);
			break;

		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
			retry(sockfd);  //重连
			break;

		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
			LOG << "connect error in Connector::startInLoop " << savedErrno;
			socket::close(sockfd);  //不能重连
			break;

		default:
			LOG << "Unexpected error in Connector::startInLoop " << savedErrno;
			socket::close(sockfd);
			// connectErrorCallback_();
			break;
	}

}


void Connector::restart(){

	m_loop->assertInLoopThread();
	setState(kDisconnected);

	m_retryDelayMs = kInitRetryDelayMs;
	m_connect = true;

	startInLoop();
}


void Connector::connecting(int sockfd){

	setState(kConnecting);
	assert(!m_channel);

	m_channel.reset(new Channel(m_loop,sockfd));
	m_channel->setWriteCallback(std::bind(&Connector::handleWrite,this));

	m_channel->setErrorCallBack(std::bind(&Connector::handleError,this));

	m_channel->enableWriting();
}

int Connector::removeAndResetChannel(){

	m_channel->disableAll();
	m_channel->remove(); //从epoll 中移除关注
	int sockfd = m_channel->fd();

	m_loop->queueInLoop(std::bind(&Connector::resetChannel,this));

	return sockfd;
}


void Connector::resetChannel() { m_channel.reset(); }

void Connector::handleWrite(){

	if(m_state == kConnecting){
	
		int sockfd = removeAndResetChannel();
		int err = socket::getSocketError(sockfd);

		if(err) //有错误
			retry(sockfd);
		else if(socket::isSelfConnect(sockfd)) //自连接
			retry(sockfd);
		else{ //连接成功

			setState(kConnected); 
			if(m_connect)	m_newConnectionCallback(sockfd);
			else socket::close(sockfd);
		}
	}else	assert(m_state == kDisconnected);
}


void Connector::handleError(){


	if(m_state == kConnecting){
		
		int sockfd = removeAndResetChannel();
		int err = socket::getSocketError(sockfd);

		retry(sockfd);
	}
}


//TODO 定时器 没有
//采用back-off 策略重连,即重连事件逐渐延长,0.5,1s,2s,4s,8s,    .. 直至30s.
void Connector::retry(int sockfd){

	socket::close(sockfd);
	setState(kDisconnected);

	if(m_connect){

	
	}

}

































































































