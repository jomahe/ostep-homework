#include <iostream>

#include <stdio.h>

using namespace std;


class ShittyVector {
   private:
      int* vec_;
      size_t size_;

   public:
      ShittyVector(int spaces) {
         // Constructor: allocate space for vector, initialize all elements to 0
         vec_ = static_cast<int*> (malloc(spaces * sizeof(int)));

         for (size_t i = 0; i < spaces; ++i) {
            vec_[i] = 0;
         }
         size_ = spaces;
      }

      ~ShittyVector() {
         delete[] vec_;
         size_ = 0;
         cout << "Resources freed" << endl;
      }

      inline void set(size_t position, int val) {
         if (position >= size_) {
            cerr << "Position is out of bounds!" << endl;
            return;
         }

         *(vec_ + position) = val;
      }

      inline int get(size_t pos) { return *(vec_ + pos); }

      inline size_t size() { return size_; }

      void push_back(int val) {
         // Realloc will automatically copy over the data we need to the new
         // memory
         vec_ = static_cast<int*> (realloc(vec_, size_ + 1));
         *(vec_ + size_) = val;
         ++size_;
      }

      void pop_back() {
         if (size_ == 0) {
            cerr << "ShittyVector object is already empty!" << endl;
            return;
         }
         vec_ = static_cast<int*> (realloc(vec_, size_ - 1));
         --size_;
      }

      inline bool empty() { return size_ == 0; }
};


int main() {
   ShittyVector v = ShittyVector(5);

   size_t vLen = v.size();

   cout << "Expected current size is five. Actual size: " << vLen << endl;
   cout << "Expect all values are currently zero; actual values: ";
   for (size_t i = 0; i < v.size(); ++i) {
      cout << v.get(i) << " ";
   }
   cout << endl;

   v.push_back(42);
   // Size should now be six, last value should be 42.
   cout << "New size: " << v.size() << ". " << "New final value: " <<
   v.get(v.size() - 1) << endl;

   v.pop_back();

   // Size should now be six, last value should be 0.
   cout << "New size: " << v.size() << ". " << "New final value: " <<
   v.get(v.size() - 1) << endl;

   return 0;
}