#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "../base/Common.h"
#include <stdio.h>
#include <iostream>

using namespace tt;
using namespace tt::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& name)
	:m_baseLoop(baseLoop),
	m_name(name),
	m_started(false),
	m_numThreads(0),
	m_next(0){
	
}

EventLoopThreadPool::~EventLoopThreadPool(){

}

void EventLoopThreadPool::start(const ThreadInitCallback& cb){

	assert(!m_started);
	m_baseLoop->assertInLoopThread();

	m_started = true;

	std::cout << " create begin " << std::endl;
	for(int i = 0 ; i < m_numThreads ; ++ i){
	
		char buf[m_name.size() + 32];
		snprintf(buf, sizeof(buf), "%s%d", m_name.c_str(), i);

		EventLoopThread* t = new EventLoopThread(cb,buf);
		m_threads.push_back(std::unique_ptr<EventLoopThread>(t));
		m_loops.push_back(t->startLoop());
	}

	if(m_numThreads == 0 && cb){
		//如果只有一个EventLoop ,在这个 EventLoop 进入事件循环之前,调用cb
		cb(m_baseLoop);
	}
	std::cout << " create down " << std::endl;
}

//一个新的连接到来,需要选择一个 EventLoop 处理
EventLoop* EventLoopThreadPool::getNextLoop(){

	m_baseLoop->assertInLoopThread();
	assert(m_started);

	// 这个 baseloop_  是 acceptor(监听套接字所属的 EventLoop), 处理连接客户端的 main_eventloop
	EventLoop* loop = m_baseLoop;

	//如果 loops_ 为空,也就上是面这个函数没有创建出线程(numThreads == 0),则 loops_ 指向 baseLoop_
	//如果不为空,按照round-robin(RR 轮叫) 的调度方式选择一个EventLoop
	if(!m_loops.empty()){
	
		loop = m_loops[m_next];
		++m_next;
		
		m_next = m_next % m_loops.size();

		/* 
		if(implicit_cast<size_t>(m_next) >= m_loops.size()){

			m_next = 0;
		}*/
	}
	
	return loop;
}

//通过 hash 获取下一个 loop
EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashcode){

	m_baseLoop->assertInLoopThread();
	EventLoop* loop = m_baseLoop;
	
	if(!m_loops.empty()){
	
		loop = m_loops[hashcode % m_loops.size()];
	}

	return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops(){

	m_baseLoop->assertInLoopThread();
	assert(m_started);

	if(m_loops.empty()){ 
		
		return std::vector<EventLoop*> (1, m_baseLoop);
	}else return m_loops;
}