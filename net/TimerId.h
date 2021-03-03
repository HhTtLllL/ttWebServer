#ifndef TT_NET_TIMERID_H
#define TT_NET_TIMERID_H

#include <stdio.h>
#include "../base/copyable.h"
#include "Callbacks.h"

namespace tt {

namespace net {

class Timer;

class TimerId : public tt::copyable {

    public: 
        TimerId() : m_timer(NULL), m_sequence(0) {

        }

        TimerId(Timer* timer, int64_t seq) : m_timer(timer), m_sequence(seq) {

        }

        friend class TimerQueue;

    private:
        Timer* m_timer;                 //定时器的地址
        int64_t m_sequence;             //定时器的序号

};

}        //net
}       //tt

#endif //TT_NET_TIOMERID_H