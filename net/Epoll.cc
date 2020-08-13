#include "Epoll.h"
#include "Channel.h"
#include "../base/Logging.h"

#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include "../base/Common.h"

using namespace tt;
using namespace tt::net;

namespace {

	const int kNew = -1;
	const int kAdded = 1;
	const int kDeleted = 2;


}

Epoll::Epoll(EventLoop* loop)
	: m_ownerLoop(loop),
	m_epollfd(::epoll_create1(EPOLL_CLOEXEC)),
	m_events(kInitEventListSize){
		if (m_epollfd < 0){
			LOG << "Epoll 初始化失败";
		}
}

Epoll::~Epoll(){

  ::close(m_epollfd);
}

void Epoll::epoll(ChannelList* activeChannels){

	//得到多少个响应
	//std::cout << "开始监听" << std::endl;
	int numEvents = ::epoll_wait(m_epollfd, &*m_events.begin(),
			static_cast<int>(m_events.size()), -1);

	int saveErrno = errno;
		
	//	std::cout << "numEvents = " << numEvents << std::endl;
	//TODO 计时
	if(numEvents > 0){
		LOG << numEvents << " events happened";


	//	std::cout << "处理活动 "  << std::endl;
		fillActiveChannels(numEvents, activeChannels);

		if( implicit_cast<size_t>(numEvents) == m_events.size() ){
			m_events.resize(m_events.size() * 2);
		}
	}else if( numEvents == 0){

		LOG << "no happened";
	}else{

		if(saveErrno != EINTR){

			errno = saveErrno;
			LOG << "Epoll::epoll";
		}
	}
}

void Epoll::fillActiveChannels(int numEvents, ChannelList* activeChannels) const {

	assert(numEvents <= m_events.size());

	for(int i = 0 ; i < numEvents ; ++ i){
	
		// ptr 里面的内容是在 update 的时候放进取的
		Channel* channel = static_cast<Channel*>(m_events[i].data.ptr);

#ifndef NDEBUG
		int fd = channel->fd();
		ChannelMap::const_iterator it = m_channels.find(fd);

		assert(it != m_channels.end());
		assert(it->second == channel);
#endif
		channel->set_revents(m_events[i].events);

		activeChannels->push_back(channel);

	}


}

//更新或者添加响应事件
void Epoll::updateChannel(Channel* channel){

	const int index = channel->index();
	int fd = channel->fd();
	if(index == kNew || index == kDeleted){

		if(index == kNew){ //index == -1 一个新的channel

			//不会出现在 map 中
			assert(m_channels.find(fd) == m_channels.end());
			//添加到map 中
			m_channels[fd] = channel;
		}else{
			//如果不是新的，一定在map中存储
			assert(m_channels.find(fd) != m_channels.end());
			assert(m_channels[fd] == channel);
		}

		channel->set_index(kAdded);

		update(EPOLL_CTL_ADD,channel);
	}else{
		//修改一个已经存在的
		int fd = channel->fd();
		(void)fd; //TODO  为什么转换呢

		assert(m_channels.find(fd) != m_channels.end());
    		assert(m_channels[fd] == channel);
    		assert(index == kAdded);

		if(channel->isNoneEvent()){

			update(EPOLL_CTL_DEL,channel);
			channel->set_index(kDeleted);

		}else{

			update(EPOLL_CTL_MOD,channel);
		}
	}
}

void Epoll::update(int operation, Channel* channel){

	struct epoll_event event;

	memset(&event, 0,  sizeof(event));

	event.events = channel->events();
	event.data.ptr = channel;

	int fd = channel->fd();

	if(::epoll_ctl(m_epollfd, operation, fd, &event) < 0){
	
		LOG << "epoll::update error";
	}
}


void Epoll::removeChannel(Channel* channel){
	
	int fd = channel->fd();
	
	//std::cout << "fd = " << fd << std::endl;
	LOG << " fd = " << fd;
	//删除的前提 肯定存在
	assert(m_channels.find(fd) != m_channels.end());
  	assert(m_channels[fd] == channel);
  	assert(channel->isNoneEvent());

	int index = channel->index();
	size_t n = m_channels.erase(fd);

	(void)n; //TODO 为什么要转化呢

	if(index == kAdded){
		
		update(EPOLL_CTL_DEL,channel);
	}

	channel->set_index(kNew);
}

bool Epoll::hasChannel(Channel* channel) const{

	ChannelMap::const_iterator it = m_channels.find(channel->fd());

	return it != m_channels.end() && it->second == channel;
}








