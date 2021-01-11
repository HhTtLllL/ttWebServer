#ifndef TT_BASE_TIMESTAMP_H
#define TT_BASE_TIMESTAMP_H 

#include "copyable.h"
//#include "Types.h"
#include <boost/operators.hpp>

namespace tt {

class Timestamp :   public tt::copyable, 
                    public boost::equality_comparable<Timestamp>, 
                    public boost::less_than_comparable<Timestamp> /*less_than_comparable  --只需要实现 <, 就可以自动推到 >, <=, >=*/{
    
    public:

        Timestamp() : m_microSecondsSinceEpoch(0) {

        }
        explicit Timestamp(int64_t microSecondsSinceEpoch) : m_microSecondsSinceEpoch(microSecondsSinceEpoch) {

        }   

        void swap(Timestamp &that) {

            std::swap(m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
        }

        std::string toString() const;
        std::string toFormattedString(bool showMicroseconds = true) const; 

        bool vaild() const { return m_microSecondsSinceEpoch > 0; }

        int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }
        time_t secondsSinceEpoch () const {

            return static_cast<time_t>(m_microSecondsSinceEpoch / kMicrosecondsPerSecond); 
        }

        static Timestamp now();
        static Timestamp invalid() { return Timestamp(); }
        static Timestamp formUnixTime(time_t t) {

            return fromUnixTime(t, 0);
        }
        
        static Timestamp fromUnixTime(time_t t, int microseconds) {

            return Timestamp(static_cast<int64_t>(t) * kMicrosecondsPerSecond + microseconds);
        }

        static const int kMicrosecondsPerSecond = 1000 * 1000;
    private:

        int m_microSecondsSinceEpoch;
};

inline bool operator< (Timestamp lhs, Timestamp rhs) {

    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {

    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low) {

    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicrosecondsPerSecond;
}

}   //tt
#endif 