#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <pthread.h>

class Counter {
private:
  int value_;
  mutable pthread_mutex_t lock_;

public:
  Counter();

  ~Counter();

  void increment();

  void decrement();

  int getValue() const;
};

#endif // COUNTER_HPP