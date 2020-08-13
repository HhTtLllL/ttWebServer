#include "CountDownLatch.h"

tt::CountDownLatch::CountDownLatch(int count)
	:m_mutex(), //构造一个 mutex对象
	m_condition(m_mutex),
	m_count(count){

}

void tt::CountDownLatch::wait(){
	MutexLockGuard lock(m_mutex);
	while(m_count > 0) { m_condition.wait(); }
}


void tt::CountDownLatch::countDown(){
	MutexLockGuard lock(m_mutex);
	--m_count;
	if(m_count == 0) m_condition.notifyAll();
}

int tt::CountDownLatch::getCount() const{
	MutexLockGuard lock(m_mutex);

	return m_count;
}
