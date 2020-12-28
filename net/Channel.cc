#include "../base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include <sstream>
#include <sys/epoll.h>
#include <iostream>

using namespace tt;
using namespace tt::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI; // POLLPRI   紧急数据
const int Channel::kWriteEvent = EPOLLOUT;


Channel::Channel(EventLoop* loop,int fd)
	:m_loop(loop),
	m_fd(fd),
	m_events(0),
	m_revents(0),
	m_index(-1),
	m_eventHandling(false),
	m_addedToLoop(false){

}

Channel::~Channel(){
	
	assert(!m_eventHandling);
	assert(!m_addedToLoop);
	//TODO 没有判断是否在该线程中
}

//调用 EventLoop::updateChannel ,
void Channel::update(){

	m_addedToLoop = true;
 
	//调用Poller::uodatechannel
	//std::cout << "Channel::update";
	m_loop->updateChannel(this);
	
}

void Channel::remove(){

	assert(isNoneEvent());

	m_addedToLoop = false;
	m_loop->removeChannel(this);
}


//当事件到来的时候，会调用 handleEvent
void Channel::handleEvent(){
	
//	std::shared_ptr<void> guard;
	handleEventWithGuard();
}

void Channel::handleEventWithGuard(){

	m_eventHandling = true;

	if((m_revents & EPOLLHUP) && !(m_revents & EPOLLIN)){

		if(m_closeCallback) m_closeCallback();
	}

	if(m_revents & (EPOLLERR)){

		if(m_errorCallback) m_errorCallback();
	}

	if(m_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
		
		if(m_readCallback) m_readCallback();
	}

	if(m_revents & EPOLLOUT){
		
		if(m_writeCallback) m_writeCallback();
	}

	m_eventHandling = false;
}