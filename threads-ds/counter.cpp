#include "counter.hpp"
#include <pthread.h>

Counter::Counter() : value_(0) { pthread_mutex_init(&lock_, NULL); }

Counter::~Counter() { pthread_mutex_destroy(&lock_); }

void Counter::increment() {
  pthread_mutex_lock(&lock_);
  ++value_;
  pthread_mutex_unlock(&lock_);
}

void Counter::decrement() {
  pthread_mutex_lock(&lock_);
  --value_;
  pthread_mutex_unlock(&lock_);
}

int Counter::getValue() const {
  pthread_mutex_lock(&lock_);
  int val = value_;
  pthread_mutex_unlock(&lock_);
  return val;
}
