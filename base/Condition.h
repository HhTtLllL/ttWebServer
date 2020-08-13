#pragma once

#include "MutexLock.h"
#include "noncopyable.h"
#include <pthread.h>
#include <time.h>

namespace tt{

class Condition : noncopyable{
public:
	explicit Condition(MutexLock& mutex)
		:m_mutex(mutex){
			pthread_cond_init(&m_cond,NULL);
	}
	~Condition(){ pthread_cond_destroy(&m_cond); }

	void wait() { pthread_cond_wait(&m_cond,m_mutex.getPthreadMutex()); }
	void notify() { pthread_cond_signal(&m_cond); }
	void notifyAll() { pthread_cond_broadcast(&m_cond); }
	
	bool waitForSeconds(int seconds);



private:
	MutexLock& m_mutex;
	pthread_cond_t m_cond;



};
}

