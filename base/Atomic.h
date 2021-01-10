#ifndef TT_BASE_ATOMIC_H
#define TT_BASE_ATOMIC_H

#include "noncopyable.h"

#include <stdint.h>

namespace tt
{

namespace detail
{

  //模仿java中原子操作的类
template<typename T>
class AtomicIntegerT : noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0){

  }

  T get(){

    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  T getAndAdd(T x){

    return __sync_fetch_and_add(&value_, x);
  }

  T addAndGet(T x){

    return getAndAdd(x) + x;
  }

  T incrementAndGet(){

    return addAndGet(1);
  }
//自减
  T decrementAndGet(){

    return addAndGet(-1);
  }
//先获取,后加
  void add(T x){

    getAndAdd(x);
  }
//自加,先加后获取
  void increment(){

    incrementAndGet();
  }
//自减,先减后获取
  void decrement(){

    decrementAndGet();
  }
//返回原来的值,然后设为新值
  T getAndSet(T newValue){

    // in gcc >= 4.7: __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST)
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:

  volatile T value_;
};

}  // detail
// 32 和 64 的整数类
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

}  // tt

#endif  // TT_BASE_ATOMIC_H