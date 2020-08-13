#include <assert.h>
#include <errno.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <iostream>
#include "Logging.h"
#include "Thread.h"
#include "CurrentThread.h"
//:#include "CurrentThread.h"
//#include "CountDownLatch.h"




/*
namespace CurrentThread {
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "default";
}
*/

namespace tt{

namespace detail{
	
	pid_t gettid(){
		//获取真实线程ID
		return static_cast<pid_t>(::syscall(SYS_gettid));
	}


/*	void tt::CurrentThread::cacheTid(){
		if(t_cachedTid == 0){
			t_cachedTid = detail::gettid();

			t_tidStringLength = snprintf(t_tidString,sizeof(t_tidString), "%5d",t_cachedTid);
		}
	}

*/	


void afterFork()
{
  tt::CurrentThread::t_cachedTid = 0;
  tt::CurrentThread::t_threadName = "main";
  CurrentThread::tid();
  // no need to call pthread_atfork(NULL, NULL, &afterFork);
}


class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    tt::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    /*
    pthread_atfork  的作用

    函数接受三个函数作为参数 
    pthread_atfork(void (*prepare)(void ), void (*parent)(void) , void (*child)(void));

    调用fork时,内部创建子进程前在父进程中会调用 prepare,内部创建子进程成功后,父进程会调用parent,子进程会调用child
    */
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

ThreadNameInitializer init;

	struct ThreadData{
		typedef tt::Thread::ThreadFunc m_ThreadFunc;
		m_ThreadFunc m_func;
		std::string m_name;
		pid_t* m_tid;
		CountDownLatch* m_latch;

		
		ThreadData(m_ThreadFunc func,const std::string& name,
				pid_t* tid,
				CountDownLatch* latch)
			:m_func(std::move(func)),
			m_name(name),
			m_tid(tid),
			m_latch(latch){
			
		}

		void runInThread(){
			//获取线程 tid
			//这里获取后在赋值为 NULL 的目的是 为了将 tid缓存到t_cachedTid 中
			*m_tid = tt::CurrentThread::tid();
			m_tid = NULL;
			m_latch->countDown();
			m_latch = NULL;
			
			tt::CurrentThread::t_threadName = m_name.empty() ? "tt_Thread" : m_name.c_str();
			
			//设置进程名
			::prctl(PR_SET_NAME,tt::CurrentThread::t_threadName);

	/*		try{
				m_func();
				tt::CurrentThread::t_threadName = "finish";
			}catch (const Exception& ex){
*/
			m_func();
			tt::CurrentThread::t_threadName = "finish";
		}
	};

	
	void* startThread(void* obj){
		
		
		ThreadData* data = static_cast<ThreadData*>(obj);

		data->runInThread();

		delete data;

		return NULL;
	}
} //detail 
	/*bool CurrentThread::isMainThread(){
		return tid() == m;
	}*/

void CurrentThread::cacheTid(){
	if(t_cachedTid == 0){
		t_cachedTid = detail::gettid();

		t_tidStringLength = snprintf(t_tidString,sizeof(t_tidString), "%5d",t_cachedTid);
	}
}


AtomicInt32 Thread::m_numCreated;


Thread::Thread(ThreadFunc func,const std::string& name)
		:m_started(false),
		m_joined(false),
		m_pthreadId(0),
		m_tid(0),
		m_func(std::move(func)),
		m_name(name),
		m_latch(1){

		setDefaultName();
}

Thread::~Thread(){
	if(m_started && !m_joined) pthread_detach(m_pthreadId);
}

void Thread::setDefaultName(){

	int num = m_numCreated.incrementAndGet();
	
	if(m_name.empty()){
		char buf[32];
		snprintf(buf,sizeof(buf),"Thread %d",num);

		m_name = buf;
	}
}


void Thread::start(){
	assert(!m_started);
	m_started = true;

	detail::ThreadData* data = new detail::ThreadData(m_func,m_name,&m_tid,&m_latch);
	
	if(pthread_create(&m_pthreadId,NULL,&detail::startThread,data)){
		m_started = false;
		delete data;

		LOG << "Failed in pthread_create" ;
	}else{
		m_latch.wait(); //阻塞, 使用信号
		assert(m_tid>0);
	}

}

int Thread::join(){
	assert(m_started);
	assert(!m_joined);
	m_joined = true;

	return pthread_join(m_pthreadId,NULL); //等待线程结束，收集资源
}




}
