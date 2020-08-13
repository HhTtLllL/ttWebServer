#pragma once

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

namespace tt{
class CountDownLatch : noncopyable{
public:
	explicit CountDownLatch(int count);

	void wait();
	void countDown(); //计数器，当计数器减为 0 是发起通知
	int getCount()const;  //获取当前计数器的值


private:
	mutable MutexLock m_mutex;
	Condition m_condition;
	int m_count;

};

}
