#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include <iostream>

using namespace std;

const int PAGE_SIZE = 4096;

inline suseconds_t getMicros() {
   struct timeval tv;
   suseconds_t micros = -1;

   if (gettimeofday(&tv, nullptr) == 0) {
      micros = tv.tv_usec;
   }
   return micros;
}

int main(int argc, char* argv[]) {
   if (argc != 3) {
      cout << "Need to specify number of pages and trials!" << endl;
      return -1;
   }

   int pages = atoi(argv[1]);
   int trials = atoi(argv[2]);

   // jump needs to be >= the number of ints on a page to guarantee a TLB miss
   int jump = ceil(static_cast<double>(PAGE_SIZE) /
                   static_cast<double>(sizeof(int)));
   size_t totalInts = jump * pages;

   volatile int* arr = static_cast<volatile int*>(malloc(totalInts));

   // Want to update one integer per page of arr
   for (size_t _ = 0; _ < trials; ++_) {
      for (size_t i = 0; i < totalInts; i += jump) {
         suseconds_t start = getMicros();
         ++arr[i];
         suseconds_t end = getMicros();

         if (i == 0) {
            cout << "Time for first access: " << end - start << endl;
         } else {
            cout << "Time between accesses to pages " << (i-1) / jump << ", "
            << i / jump << ": " << end - start << endl;
         }
      }
   }

   delete[] arr;

   return 0;
}