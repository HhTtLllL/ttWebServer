#include "Timestamp.h"
#include <stdio.h>
#include <sys/time.h>


using namespace tt;

static_assert(sizeof(Timestamp) == sizeof(int64_t));

/* 
std::string Timestamp::toString() const {

    char buf[32] = {0};
    int64_t seconds = m_microSecondsSinceEpoch / kMicrosecondsPerSecond;
    int64_t microseconds = m_microSecondsSinceEpoch % kMicrosecondsPerSecond;

    snprintf(buf, sizeof(buf) - 1, "%", seconds, microseconds);

    return buf;
}
*/

Timestamp Timestamp::now() {

    struct timeval tv;

    gettimeofday(&tv, NULL);

    //tv_sec 秒数 , tv_usec 微秒数
    return Timestamp((int64_t)tv.tv_sec * kMicrosecondsPerSecond + tv.tv_usec);
}