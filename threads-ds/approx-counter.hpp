#include <pthread.h>

class ApproximateCounter {
private:
  int globalVal_;
  pthread_mutex_t glock_;
  int* localVal_;
  pthread_mutex_t* llock_;
  int threshold_;
  int cpus_;

public:
  ApproximateCounter(int threshold);
  ~ApproximateCounter();

  void update(int threadId, int amount);
  int getValue();
}