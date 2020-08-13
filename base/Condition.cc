#include "Condition.h"
#include <error.h>
#include <iomanip>

bool tt::Condition::waitForSeconds(int seconds){
	struct timespec abstime;

	clock_gettime(CLOCK_REALTIME,&abstime);
	
	//有待思考 TODO
	abstime.tv_sec += static_cast<time_t>(seconds);

	return ETIMEDOUT == pthread_cond_timedwait(&m_cond,m_mutex.getPthreadMutex(),&abstime);

}

