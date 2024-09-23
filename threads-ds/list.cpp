#include <pthread.h>

#include "list.hpp"

List::List() {
  head = new ListNode();
  head->next_ = nullptr;
  pthread_mutex_init(&head->lock_, NULL);
}

List::~List() {
  ListNode* curr = head;

  while (curr) {
    pthread_mutex_destroy(&curr->lock_);
    curr = curr->next_;
  }
}

void List::insert(int value) {
  ListNode* curr = head;
  ListNode* next = head->next_;

  pthread_mutex_lock(&curr->lock_);

  while (next) {
    pthread_mutex_lock(&next->lock_);

    // Do nothing; not performing a node-level edit

    pthread_mutex_unlock(&curr->lock_);
    curr = next;
    next = next->next_;
  }

  // curr->next_ should now be null
  curr->next_ = new ListNode();
  curr->next_->val_ = value;
  pthread_mutex_init(&curr->next_->lock_, NULL);

  pthread_mutex_unlock(&curr->lock_);
}

List::ListNode* List::lookup(int value) {
  ListNode* found = nullptr;

  ListNode* curr = head;
  ListNode* next = head->next_;

  pthread_mutex_lock(&curr->lock_);
  while (curr->next_) {
    pthread_mutex_lock(&next->lock_);

    if (curr->val_ == value) {
      found = curr;
      break;
    }
    pthread_mutex_unlock(&curr->lock_);

    curr = next;
    next = next->next_;
  }
  pthread_mutex_unlock(&curr->lock_);

  return found;
}
