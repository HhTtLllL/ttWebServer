#ifndef TT_NET_EVENTLOOP_H
#define TT_NET_EVENTLOOP_H

#include <atomic>
#include <functional>
#include <vector>
#include <boost/any.hpp>
#include "../base/MutexLock.h"
#include "../base/CurrentThread.h"
#include <pthread.h>
#include <memory>

//#include "../net/Callbacks.h"
//#include "../net/TimerId.h"

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

namespace tt{

namespace net{

class Channel;
class Epoll;


class EventLoop : noncopyable{


public:
	typedef std::function<void()> Functor;

	EventLoop();
	~EventLoop();

	void loop();
	void quit();

	void runInLoop(Functor cb);
	void queueInLoop(Functor cb);

	size_t queueSize() const;

	//TODO  定时器
	
	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

	bool isInLoopThread() const { return m_threadId == CurrentThread::tid(); }

	void assertInLoopThread() { 
	
		if(!isInLoopThread()) abort();
	}

	bool eventHanding() const { return m_eventHandling; }

	static EventLoop* getEventLoopOfCurrentThread();

private:

	void abortNotInLoopThread();
	void handleRead();  // waked up
	void doPendingFunctors();

	typedef std::vector<Channel*> ChannelList;
	
	bool m_looping;    //是否正在执行loop循环
	std::atomic<bool> m_quit;  //是否已经调用quit()　函数退出loop 循环
	bool m_eventHandling;    //enentHandling 是否正在处理event 事件
	bool m_callingPendingFunctors;  //是否正在调用pendingFunctors_ 的函数对象

	const pid_t m_threadId;   //当前线程所属ID

	std::unique_ptr<Epoll> m_epoll;//生成期由EventLoop 控制

	int m_wakeupFd;
	std::unique_ptr<Channel> m_wakeupChannel; //wake 通道，该通道也会纳入epoll 来管理
  
	ChannelList m_activeChannels;   //记录这激活事件的集合　　　　Epoll 返回的活动通道
	Channel* m_currentActiveChannel;  //当前正在处理的channel 事件　　当前正在处理的活动通
	mutable MutexLock m_mutex;
	
//	THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
//	表示变量 m_pendingFunctors 被锁保护
	std::vector<Functor> m_pendingFunctors  THREAD_ANNOTATION_ATTRIBUTE__(guatded_by(m_mutex));  //是当前线程要执行的任务的集



};


}// net


}//tt



#endif
