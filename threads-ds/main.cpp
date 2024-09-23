#include <sys/time.h>
#include <cassert>
#include <iostream>

#include "counter.hpp"

using namespace std;

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

int main() {
  // counter seems to scale linearly
  // testCounter(500);  // Time taken for 500 inserts: 15
  // testCounter(5000);  // Time taken for 5000 inserts: 144
  // testCounter(50000);  // Time taken for 50000 inserts: 1442

  return 0;
}