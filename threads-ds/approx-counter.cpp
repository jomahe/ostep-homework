#include <thread>
#include "approx-counter.hpp"

using namespace std;

ApproximateCounter::ApproximateCounter(int threshold)
  : globalVal_(0)
  , cpus_(thread::hardware_concurrency())
  , threshold_{0} {

    pthread_mutex_init(&glock_, NULL);
    localVal_ = static_cast<int*>(malloc(cpus_ * sizeof(int)));
    llock_ = static_cast<pthread_mutex_t*>(malloc(cpus_ * sizeof(pthread_mutex_t)));
}

ApproximateCounter::~ApproximateCounter() {
  for (size_t i = 0; i < cpus_; ++i) {
    pthread_mutex_destroy(&llock_[i]);
  }
  pthread_mutex_destroy(&glock_);

  free(localVal_);
  free(llock_);
}

void ApproximateCounter::update(int threadId, int amount) {
  int slot = threadId % cpus_;
  pthread_mutex_lock(&llock_[slot]);

  localVal_[slot] += amount;

  if (localVal_[slot] >= threshold_) {
    pthread_mutex_lock(&glock_);
    globalVal_ += localVal_[slot];
    pthread_mutex_unlock(&glock_);

    localVal_[slot] = 0;
  }

  pthread_mutex_unlock(&llock_[slot]);
}

int ApproximateCounter::getValue() {
  pthread_mutex_lock(&glock_);
  int gVal = globalVal_;
  pthread_mutex_unlock(&glock_);

  return gVal;
}
