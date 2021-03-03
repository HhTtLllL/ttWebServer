#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS

#endif 

#include "TimerQueue.h"
#include "../base/Logging.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"

#include <sys/timerfd.h>
#include <unistd.h>

namespace tt {

namespace net {

namespace detail {

//创建一个定时器
int createTimerfd() {

     int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
     if(timerfd < 0) {

         LOG << "Failed in timerfd_create";
     }

     return timerfd;
}


//清除定时器， 避免一直触发
void readTimerfd(int timerfd, Timestamp now) {

    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));

    if(n != sizeof(howmany)) {

        LOG << "TiemrQueue::handleRead()" << n << "bytes instrad of 8";
    }
}

//重置定时器的超时时间
void resetTimerfd(int timerfd, Timestamp expiration) {

    
}

}   //detail

}   //net

}   //tt

using namespace tt;
using namespace tt::net;
using namespace tt::net::detail;


TimerQueue::TimerQueue(EventLoop* loop) 
            : m_loop(loop) ,
            m_timerfd(createTimerfd()), 
            m_timerfdChannel(loop, m_timerfd),
            m_timers(),
            m_callingExpiredTimers(false) {

                //当定时器通道可读事件产生的时候， 会回调handleRead 

            m_timerfdChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this));

            m_timerfdChannel.enableReading();
}

TimerQueue::~TimerQueue() {

    m_timerfdChannel.disableAll();
    m_timerfdChannel.remove();
    ::close(m_timerfd);

    for( const Entry& timer : m_timers) {

        delete timer.second;
    }
}
                                                //超时时间              时间间隔
TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp when, double interval) {

    //构造一个新的定时器对象
    Timer* timer = new Timer(std::move(cb), when, interval);
    m_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));

    return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId) {

    m_loop->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer) {

    m_loop->assertInLoopThread();

    //插入一个定时器可能会使得最早到期的定时器发生改变
    bool earliestChanged = insert(timer);
    if(earliestChanged) {

        //重置定时器的超时时刻
        resetTimerfd(m_timerfd, timer->expiration());
    }
}

void TimerQueue::cancelInLoop(TimerId timerId) {

    m_loop->assertInLoopThread();

    ActiveTimer timer(timerId.m_timer, timerId.m_sequence);

    //查找该定时器
    auto it = m_activeTimers.find(timer);
    if(it != m_activeTimers.end()) {

        size_t n = m_timers.erase(Entry(it->first->expiration(), it->first));

        delete it->first;                                           //如果用unique_ptr 这里就不需要手动删除了
        m_activeTimers.erase(it);
    }else if(m_callingExpiredTimers) {

        //已经到期， 并且正在调用回调函数的定时器 
        m_cancelingTimers.insert(timer);
    }
}

void TimerQueue::handleRead(){ 
    
    m_loop->assertInLoopThread();

    Timestamp now(Timestamp::now());
    //清除该事件， 避免一直触发
    readTimerfd(m_timerfd, now);

    std::vector<Entry> expired = getExpired(now);

    m_callingExpiredTimers = true;
    m_cancelingTimers.clear();

    for(const Entry& it : expired) {

        it.second->run();
    }

    m_callingExpiredTimers = false;

    //不是一次性定时器， 需要重置下次定时时间
    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now) {

    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));

    //返回第一个未到期的Timner的迭代器， 就意味着这个迭代器之前的元素全部已经到期了 
    auto end = m_timers.lower_bound(sentry);

    //将到期的定时器插入到expired中
    std::copy(m_timers.begin(), end, back_inserter(expired));

    //从timers中移除到期的定时器 
    m_timers.erase(m_timers.begin(), end);

    //从activeTimers 中一出移除到期的定时器
    for(const Entry& it : expired) {

        ActiveTimer timer(it.second, it.second->sequence());
        size_t n = m_activeTimers.erase(timer);
    }

    return expired;
}


void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now) {
    
    Timestamp nextExpire;

    for(const Entry& it : expired) { 

        ActiveTimer timer(it.second, it.second->sequence());

        //如果是重复的定时器并且是未取消定时器，则重启该定时器
        if( it.second->repeat() && m_cancelingTimers.find(timer) == m_cancelingTimers.end()) {

            it.second->restart(now);
            insert(it.second);
        }else {

            //一次性定时器是不能重复的， 因此删除该定时器
            delete it.second;
        }
    }

    if(!m_timers.empty()) {

        nextExpire = m_timers.begin()->second->expiration();
    }

    if(nextExpire.valid()) {

        resetTimerfd(m_timerfd, nextExpire);
    }
}


bool TimerQueue::insert(Timer* timer) {

    //判断是否在I/O线程中调用
    m_loop->assertInLoopThread();

    //最早到期时间是否改变
    bool earliestChanged = false;
    Timestamp when = timer->expiration();

    auto it = m_timers.begin();
    if(it == m_timers.end() || when < it->first) {

        earliestChanged = true;
    }

    {
        std::pair<TimerList::iterator, bool> result = m_timers.insert(Entry(when, timer));
    }

    {
        std::pair<ActiveTimerSet::iterator, bool> result = m_activeTimers.insert(ActiveTimer(timer, timer->sequence()));
    }

    return earliestChanged;
}



