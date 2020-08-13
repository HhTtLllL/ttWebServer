#pragma once

#include <pthread.h>

#include "noncopyable.h"

namespace tt{
class MutexLock : noncopyable{
public:
	/*MutexLock() 
	: m_holder(0){ 
		pthread_mutex_init(&m_mutex,NULL);
	}*/

	MutexLock() { pthread_mutex_init(&m_mutex,NULL); }

	~MutexLock(){
		pthread_mutex_lock(&m_mutex);
		pthread_mutex_unlock(&m_mutex);
	}

	void lock() { pthread_mutex_lock(&m_mutex); }
	void unlock() { pthread_mutex_unlock(&m_mutex); }
	pthread_mutex_t* getPthreadMutex() { return &m_mutex; }

private:
	pthread_mutex_t m_mutex;
	pid_t m_holder;


private:
	friend class Condition;


};

class MutexLockGuard : noncopyable{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		:m_mutex(mutex){
			m_mutex.lock();
		}

	~MutexLockGuard(){ m_mutex.unlock(); }
private:
	MutexLock& m_mutex;   //这里是引用

};

}
