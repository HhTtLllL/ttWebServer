#pragma once

#include <functional>
#include <string>
#include <vector>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"


namespace tt{

class AsyncLogging : noncopyable{

public:
					           //每隔长时间，调用一次日志函数
	AsyncLogging(const std::string& basename, int flushInterval = 2);
	~AsyncLogging(){
		if(m_running) stop();
	}

	void append(const char* logline, int len);

	void start(){
		m_running = true;
		m_thread.start();
		m_latch.wait();
	}


	void stop(){
		m_running = false;
		m_cond.notify();
		m_thread.join();
	}



private:
	//回调函数
	void threadFunc();

	typedef detail::FixedBuffer<detail::kLargeBuffer> Buffer;
	typedef std::vector<std::shared_ptr<Buffer> > BufferVector;
	typedef std::shared_ptr<Buffer> BufferPtr;
	const int m_flushInterval;

	bool m_running;
	std::string m_basename;
	tt::Thread m_thread;
	tt::CountDownLatch m_latch;
	tt::MutexLock m_mutex;
	tt::Condition m_cond;

	BufferPtr m_currentBuffer;
	BufferPtr m_nextBuffer;
	BufferVector m_buffers;
};


}// tt
