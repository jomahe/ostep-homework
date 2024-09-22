#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

using namespace std;


inline suseconds_t getMicros() {
   struct timeval tv;
   suseconds_t micros = -1;

   if (gettimeofday(&tv, nullptr) == 0) {
      micros = tv.tv_usec;
   }
   return micros;
}

void p1() {
   // Part 1: repeatedly measure the time it takes for 0-byte reads using
   // gettimeofday()

   // Output:
   // time taken for read 1 : 3 microseconds
   // time taken for read 2 : 2 microseconds
   // time taken for read 3 : 2 microseconds
   // time taken for read 4 : 2 microseconds
   // time taken for read 5 : 1 microseconds
   // time taken for read 6 : 2 microseconds
   // time taken for read 7 : 1 microseconds
   // time taken for read 8 : 1 microseconds
   // time taken for read 9 : 1 microseconds
   // time taken for read 10 : 1 microseconds

   // Seems like the reads following the first are much quicker despite the
   // files being read from being different. Why is this? It's also worth noting
   // that the granularity may not be enough and the 1 microsecond figure is
   // actually just the ceiling of the amount of time actually taken.



   int counter = 10;
   while(counter--) {
      string filename = "example" + to_string(counter) + ".txt";
      int fd = open(filename.c_str(), O_CREAT | O_RDONLY);

      if (fd < 0) {
         cout << "bad file descriptor" << endl;
         return;
      }
      suseconds_t start = getMicros();
      read(fd, nullptr, 0);
      suseconds_t end = getMicros();

      cout << "time taken for read " << 10 - counter << " : " << end - start <<
      " microseconds " << endl;
   }
}


int main() {
   p1();

   return 0;
}