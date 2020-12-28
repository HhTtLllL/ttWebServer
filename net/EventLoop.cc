#include "EventLoop.h"
#include "../base/Logging.h"
#include "../base/MutexLock.h"
#include "Channel.h"
#include "Epoll.h"
#include "SocketsOps.h"

#include <algorithm>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>

using namespace tt;
using namespace tt::net;

//当前线程EventLoop 对象指针， 线程局部存储
__thread EventLoop* t_loopInThisThread = 0;

const int kEpollTmeMs = 10000;

int createEventfd(){
	
	//创建一个eventfd对象, 可以由用户空间应用程序实现事件等待/通知机制. 或由内核通知用户应用程序事件
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  
	if (evtfd < 0){

		LOG << "Failed in eventfd";
		abort();
	}

	return evtfd;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread(){
	
	return t_loopInThisThread;
}

EventLoop::EventLoop()
	:m_looping(false),
	m_quit(false),
	m_eventHandling(false),
	m_callingPendingFunctors(false),
	m_threadId(CurrentThread::tid()),
	m_epoll(new Epoll(this)),
	m_wakeupFd(createEventfd()),
	m_wakeupChannel(new Channel(this,m_wakeupFd)),
	m_currentActiveChannel(nullptr){

	std::cout << "EventLoop::EventLoop" << std::endl;

		//如果这个线程已经存在了这样一个 loop Eventloop 对象, 就不在创建
		if(t_loopInThisThread){
			
			LOG << "该线程已经创建eventloop对象";
		}else{

			t_loopInThisThread = this;
		}

		m_wakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead,this));
		m_wakeupChannel->enableReading();
}

EventLoop::~EventLoop(){

	m_wakeupChannel->disableAll();
	m_wakeupChannel->remove();
	::close(m_wakeupFd);

	t_loopInThisThread = NULL;
}

//事件循序, 该函数不能跨线程调用
void EventLoop::loop(){

	assert(!m_looping);
	assertInLoopThread();

	m_looping = true;
	m_quit = false;

	while(!m_quit){
		
		m_activeChannels.clear();
		m_epoll->epoll(&m_activeChannels);					//当IO不是很繁忙的时候，处于阻塞状态．阻塞在这里，　此时也能执行一些计算任务
		m_eventHandling = true;  

		//遍历活动通道，进行处理
		for(Channel* channel : m_activeChannels){

			m_currentActiveChannel = channel;
			m_currentActiveChannel->handleEvent();
		}

		m_currentActiveChannel = NULL;
		m_eventHandling = false;

		doPendingFunctors();
	}

	m_looping  = false;
}

void EventLoop::quit(){
	
	m_quit = true;
	
	if(!isInLoopThread()){
	
		wakeup();
	}
}

//在 io 线程中执行某个回调函数，　该函数可以跨线程调用
void EventLoop::runInLoop(Functor cb){
	
	if(isInLoopThread()) cb();
	else{
	
		//如果是其他线程调用 runInLoop,则异步地将cb添加到队列中
		// 让EventLoop 所对应的IO  线程 执行相应的 回调函数函数
		queueInLoop(std::move(cb));
	}
}

void EventLoop::queueInLoop(Functor cb){

	{
		MutexLockGuard lock(m_mutex);
		m_pendingFunctors.push_back(std::move(cb));
	}

	if(!isInLoopThread() || m_callingPendingFunctors ){
	
		wakeup();
	}
}

size_t EventLoop::queueSize() const{

	MutexLockGuard lock(m_mutex);

	return m_pendingFunctors.size();
}

void EventLoop::updateChannel(Channel* channel){

	assert(channel->ownerLoop() == this);
	assertInLoopThread();

	m_epoll->updateChannel(channel);
	/*
	if(m_eventHandling){
	
		assert(m_currentActiveChannel == channel ||
		std::find(m_activeChannels.begin(),m_activeChannels.end(), channel) ==  m_activeChannels.end());
	}

	m_epoll->removeChannel(channel);*/
}


void EventLoop::removeChannel(Channel* channel){

	assert(channel->ownerLoop() == this);
	assertInLoopThread();

	if(m_eventHandling){
	
		assert(m_currentActiveChannel == channel || 
				std::find(m_activeChannels.begin(), m_activeChannels.end(), channel) == m_activeChannels.end());
	}

	m_epoll->removeChannel(channel);
}


bool EventLoop::hasChannel(Channel* channel){

	assert(channel->ownerLoop() == this);

	return m_epoll->hasChannel(channel);
}

void EventLoop::wakeup(){

	uint64_t one = 1;

	//m_wakeupFd 的类型是  eventfd  , eventf 只有  8 个字节
	ssize_t n = socket::write(m_wakeupFd, &one, sizeof(one));
	//唤醒另一个线程 往这个线程中写入 8 个字节,就可以唤醒这个线
	//sockets::write(m_wakeupFd,&one,sizeof(one));
}


void EventLoop::handleRead(){

	uint64_t one = 1;
	//再把 之前写入的 8 个字节 读出
	ssize_t n = socket::read(m_wakeupFd, &one, sizeof(one));
	//sockets::read(m_wakeupFd,&one,sizeof(one));
}

void EventLoop::doPendingFunctors(){

	std::vector<Functor> functors;

	m_callingPendingFunctors = true;

	{
		MutexLockGuard lock(m_mutex);
		//swap 调用完之后，pending 中就为空
		//缩小了临界区的长度，　也避免了死锁，因为functor 可能在此调用queueinloop 
		functors.swap(m_pendingFunctors);
	}

	for(const Functor& functor : functors){
		
		functor();
	}

	m_callingPendingFunctors = false;
}