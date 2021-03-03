#ifndef TT_NET_TIMEQUEUE_H
#define TT_NET_TIMEQUEUE_H

#include <set>
#include <vector>

#include "../base/MutexLock.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"
#include "Channel.h"

namespace tt {

namespace net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : tt::noncopyable {


    public: 

        explicit TimerQueue(EventLoop* loop);
        ~TimerQueue();

        TimerId addTimer(TimerCallback cb, Timestamp when, double interval);
        void cancel(TimerId timerId);

    private: 

        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;              //按照时间戳排序
        typedef std::pair<Timer*, int64_t> ActiveTimer;
        typedef std::set<ActiveTimer> ActiveTimerSet;

        void addTimerInLoop(Timer* timer);
        void cancelInLoop(TimerId timerId);

        void handleRead();
        std::vector<Entry> getExpired(Timestamp now);

        //对超时的定时器重置
        void reset(const std::vector<Entry> & expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* m_loop;              //所属的EventLoop
        const int m_timerfd;
        Channel m_timerfdChannel;       //是timerfd创建的通道， 当定时器的时间到来时， 产生可读时间，回调handleRead 
        TimerList m_timers;             //按照时间排序
        
        ActiveTimerSet m_activeTimers;
        bool m_callingExpiredTimers;    //是否处于调用处理超时定时器中

        ActiveTimerSet m_cancelingTimers;       //保存的是被取消的定时器
};

}    //net

}   //tt

#endif //TT_NET_TIMEQUEUE_H