#ifndef TT_NET_EPOLL_H
#define TT_NET_EPOLL_H

#include <vector>
#include <map>
#include "EventLoop.h"


struct epoll_event;

namespace tt
{

namespace net
{

class Channel;

class Epoll
{

public:
	typedef std::vector<Channel*> ChannelList;

	Epoll(EventLoop* loop);
	~Epoll();

	void epoll(ChannelList* activeChannels);
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool  hasChannel(Channel* channel) const;



private:
	static const int kInitEventListSize = 16;

	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
	void update(int operation, Channel* channel);

	typedef std::vector<struct epoll_event> EventList;

	int m_epollfd;
	EventList m_events;


	typedef std::map<int, Channel*> ChannelMap;
	ChannelMap m_channels;

	EventLoop* m_ownerLoop; //epoll 会记录它所登记的 EventLoop

};

}// net


}//tt

#endif
