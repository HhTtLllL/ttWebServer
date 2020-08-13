#include "EventLoopThread.h"
#include "EventLoop.h"
#include <iostream>

using namespace tt;
using namespace tt::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
		const std::string& name)
	:m_loop(NULL),
	m_exting(false),
	m_thread(std::bind(&EventLoopThread::threadFunc, this), name),
	m_mutex(),
	m_cond(m_mutex),
	m_callback(cb){
	
}

EventLoopThread::~EventLoopThread(){

	m_exting = true;

	if(m_loop != NULL){
	
		m_loop->quit();
		m_thread.join();
	}
}

EventLoop* EventLoopThread::startLoop(){

	assert(!m_thread.started());


	m_thread.start();

	EventLoop* loop = NULL;
	
	{

		MutexLockGuard lock(m_mutex);

		while(m_loop == NULL){
		
			m_cond.wait();
		}
		loop = m_loop;
	}

	return loop;
}

void EventLoopThread::threadFunc(){

	EventLoop loop;

	if(m_callback) m_callback(&loop);

	{

		MutexLockGuard lock(m_mutex);

		m_loop = &loop;

		m_cond.notify();
	}

	loop.loop();

	MutexLockGuard lock(m_mutex);
	
	m_loop = NULL;
}




