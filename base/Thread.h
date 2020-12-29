#pragma once

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <memory>
#include <functional>
#include <string>
#include "noncopyable.h"
#include "CountDownLatch.h"
#include "../base/Atomic.h"

namespace tt{

class Thread : noncopyable{

public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(ThreadFunc func, const std::string& name = std::string());
	~Thread();

	void start();
	int join();

	bool started() const { return m_started; }
	pid_t tid() const { return m_tid; }
	const std::string& name()const  { return m_name; }

private:
	void setDefaultName(); //给线程设置默认名称

	bool m_started; //线程是否启动
	bool m_joined;

	pthread_t m_pthreadId;
	pid_t m_tid; //线程真实id
	ThreadFunc m_func; //线程回调函数
	std::string m_name; //线程名称

	static AtomicInt32 m_numCreated;
	CountDownLatch m_latch;  //已经创建的线程的个数
};

}