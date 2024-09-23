#include <pthread.h>

class List {

struct ListNode {
  int val_;
  ListNode* next_;
  pthread_mutex_t lock_;
};

private:
  ListNode* head;

public:
  List();
  ~List();

  void insert(int value);
  ListNode* lookup(int value);
};
