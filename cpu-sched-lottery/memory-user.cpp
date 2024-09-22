#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

using namespace std;

void memory_user() {

}

int main(int argc, char* argv[]) {
   if (argc != 2) {
      cout << "improper usage" << endl;
      return 1;
   }

   int mb = atoi(argv[1]);
   int numIntegers = (mb * 1024 * 1024) / sizeof(int);

   int* array = new int[numIntegers];

   for (size_t i = 0; i < numIntegers; ++i) {
      array[i] = 0;
      if (i == numIntegers / 2) {
         cout << getpid() << endl;
         cout << "try checking available memory" << endl;
         cin.get();
      }
   }

   delete[] array;

   return 0;
}