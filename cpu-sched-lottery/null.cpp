#include <iostream>

#include <stdlib.h>

using namespace std;

int main() {
   // int* x = (int*) malloc(sizeof(int));

   // *x = 5;

   int* data = (int*) malloc(100 * sizeof(int));
   free(data + 5);
   cout << data[2] << endl;
   cout << data[6] << endl;
   // cout << *x << endl;

   return 0;
}