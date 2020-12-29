#ifndef TT_NET_EVENTLOOPTHREADPOOL_H
#define TT_NET_EVENTLOOPTHREADPOOL_H

#include "../base/noncopyable.h"
#include <functional>
#include <memory>
#include <vector>
#include <iostream>

namespace tt
{

namespace net
{

class EventLoop;
class EventLoopThread;

//  IO 线程池的功能是开启若干个IO 线程,并让这些IO 线程处于事件循环的状态
class EventLoopThreadPool : noncopyable{
	

public:
	typedef std::function<void (EventLoop*)> ThreadInitCallback;

	EventLoopThreadPool(EventLoop* baseloop, const std::string& name);
	~EventLoopThreadPool();

	void setThreadNum(int numThreads) {	m_numThreads = numThreads; }
	void start(const ThreadInitCallback& cb = ThreadInitCallback());

	EventLoop* getNextLoop();
	EventLoop* getLoopForHash(size_t hashCode);

	std::vector<EventLoop*> getAllLoops();

	bool started() const{ return m_started; }
	const std::string& name() const { return m_name; }

private:
	EventLoop* m_baseLoop;
	std::string m_name;
	bool m_started;
	int m_numThreads;
	int m_next;

	std::vector<std::unique_ptr<EventLoopThread>> m_threads;
	//这里的是栈上对象,不需要我们来管理
	std::vector<EventLoop*> m_loops; //EventLoop 列表   一个IO 线程,对应一个 EventLoop 对象
};

}//net

} //tt

#endif