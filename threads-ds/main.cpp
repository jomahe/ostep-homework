#include <sys/time.h>
#include <cassert>
#include <iostream>

#include "counter.hpp"
#include "list.hpp"

using namespace std;

const int NUM_THREADS = 8;
const int NUM_INSERTS = 10000;

List sharedList;

void* threadInsert(void* arg) {
  int threadId = *(static_cast<int*>(arg));

  for (size_t i = 0; i < NUM_INSERTS; ++i) {
    sharedList.insert(threadId * NUM_INSERTS + i);
  }

  pthread_exit(NULL);
}

inline suseconds_t getMicros() {
   struct timeval tv;
   suseconds_t micros = -1;

   if (gettimeofday(&tv, nullptr) == 0) {
      micros = tv.tv_usec;
   }
   return micros;
}

void testCounter(int numInserts) {
  Counter c = Counter();

  suseconds_t start = getMicros();
  for (size_t i = 0; i < numInserts; ++i) {
    c.increment();
  }
  suseconds_t end = getMicros();
  assert(c.getValue() == numInserts);

  cout << "Time taken for " << numInserts << " inserts: "<< end - start << endl;
}

void testList() {

}

int main() {
  // counter seems to scale linearly
  // testCounter(500);  // Time taken for 500 inserts: 15
  // testCounter(5000);  // Time taken for 5000 inserts: 144
  // testCounter(50000);  // Time taken for 50000 inserts: 1442

  pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    // Start time measurement
    auto start = std::chrono::high_resolution_clock::now();

    // Create threads to perform insertions
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadIds[i] = i;
        int rc = pthread_create(&threads[i], NULL, threadInsert, &threadIds[i]);
        if (rc) {
            std::cerr << "Error: Unable to create thread " << rc << std::endl;
            exit(-1);
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // End time measurement
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Output the total time taken
    std::cout << "Time taken with " << NUM_THREADS << " threads: "
              << elapsed.count() << " seconds" << std::endl;

    return 0;

  return 0;
}