#include "Timer.h"

using namespace tt;
using namespace tt::net;

AtomicInt64 Timer::m_s_numCreated;

void Timer::restart(Timestamp now) {

    if(m_repeat) {

        //如果是重复性定时器， 重新计算下一个超时时间
                                //当前时间 + 时间间隔
        m_expiration = addTime(now, m_interval);
    }else {

        //非重复定时器
        m_expiration = Timestamp::invalid();
    }
}