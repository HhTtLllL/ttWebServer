#pragma once 

#include <stdint.h>

namespace tt{

namespace CurrentThread{

	//__thread 保证了每个线程都有自己的一份
	extern __thread int t_cachedTid;//缓存 tid
	extern __thread char t_tidString[32];  //格式化为 字符串
	extern __thread int t_tidStringLength; //字符串线程的长度
	extern __thread const char* t_threadName; //线程名称

	void cacheTid();
	bool isMainThread();



	inline int tid(){

		if(__builtin_expect(t_cachedTid == 0,0)){

			cacheTid();
		}

		return t_cachedTid;
	}

	inline int tidStringLength() { return t_tidStringLength; }
	inline const char* name() { return t_threadName; }
	inline const char* tidString() { return t_tidString; }

}  //CurrentThread

}  //tt
