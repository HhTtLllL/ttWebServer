#ifndef TT_NET_EVENTLOOPTHREAD_H
#define TT_NET_EVENTLOOPTHREAD_H

#include "../base/Condition.h"
#include "../base/MutexLock.h"
#include "../base/Thread.h"
#include "../base/Common.h"

namespace tt
{


namespace net
{

class EventLoop;

class EventLoopThread : noncopyable{


public:
	typedef std::function<void(EventLoop*)> ThreadInitCallback;

	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const std::string& name = std::string());

	~EventLoopThread();

	 
	//启动线程,调用thread_ 中的start() ,start 启动 threadfunc . 该线程就成为了IO线程 在线程中创建一个 EventLoop 对象，然后将llop 指针，指向
	// EventLoop 对象
	EventLoop* startLoop();

private:

	//线程回调函数
	void threadFunc();

	//指向 一个eventloop 对象
	EventLoop* m_loop THREAD_ANNOTATION_ATTRIBUTE__(guatded_by(m_mutex));

	bool m_exting;

	Thread m_thread; 
	MutexLock m_mutex;
	Condition m_cond THREAD_ANNOTATION_ATTRIBUTE__(guatded_by(m_mutex));

	ThreadInitCallback m_callback;
};


}// net

}// tt





#endif
