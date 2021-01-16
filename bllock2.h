#ifndef BLLOCK2
#define BLLOCK2

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>

#include <mutex>
#include "Node.h"

class bllock2{
  public:
    void enqueue(int x){
      enqlock.lock();

      Node *e = new Node(x);
      tail->next = e;
      tail = e;
      enqlock.unlock();

    }

    bllock2(){
      // std::cout << "INITIALIZE BLLOCK\n";
      Node *a = new Node();
      head = a;
      tail = a;
      root = a;
    }
    ~bllock2(){
      Node *curr = root;
      while(curr->next != nullptr)
      {
        Node *prev = curr;
        curr = curr->next;
        if(curr != nullptr)
          delete curr;
      }
    }
    int dequeue(){
      int x;
      deqlock.lock();

      if(head->next == nullptr)
      {
        deqlock.unlock();
        return(-1);
      }

      x = head->next->value;
      head = head->next;

      deqlock.unlock();
      return(x);
    }
  private:
      Node *tail, *head, *root;
      std::mutex enqlock, deqlock;

      friend std::ostream& operator<<(std::ostream& os, const bllock2& me)
      {
        Node *curr = (me.head);
        if(curr->next == nullptr)
        {
          os << "Queue is empty\n";
          return(os);
        }

        while(curr->next != nullptr)
        {
          os << curr->next->value << "\n";
          curr = curr->next;
        }

        os << "End of queue\n";
        return(os);
      }
};

#endif
