#ifndef TT_NET_TIMER_H
#define TT_NET_TIMER_H

#include "../base/Atomic.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"

namespace tt {

namespace net {

class Timer : tt::noncopyable {

    public: 
        Timer(TimerCallback cb, Timestamp when, double interval)
           : m_callback(std::move(cb)),
            m_expiration(when), 
            m_interval(interval),
            m_repeat(interval > 0.0), 
            m_sequence(m_s_numCreated.incrementAndGet()) 
              {

        }

        void run (void) {

            m_callback();           //调用回调函数
        }

        Timestamp expiration() const { return m_expiration; }
        bool repeat () const { return m_repeat; }
        int64_t sequence() const { return m_sequence; }
        
        //重启
        void restart(Timestamp now);
        static int64_t numCreated() { return m_s_numCreated.get(); }


    private:

        const TimerCallback m_callback;                 //定时器的回调函数
        Timestamp m_expiration;                         //下一次的超时时刻
        const double m_interval;                        //超时时间间隔，当超时时刻来临时， 调用定时器回调函数
        const bool m_repeat;                            //定时器是否重复
        const int64_t m_sequence;                       //定时器序号， 每一个定时器有一个唯一的编号

        static AtomicInt64 m_s_numCreated;              //定时器技术,当前已经创建的定时器的数量
};

}       //net

}       //tt

#endif 