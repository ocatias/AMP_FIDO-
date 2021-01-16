#ifndef NODE
#define NODE

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>
// #include <recursive_mutex>
#include <mutex>
#include <condition_variable>

class Node{
  public:
    Node* next;
    int value;

    Node(int x)
    {
      value = x;
      next = nullptr;

    }
    Node(){
      next = nullptr;
    }
};

#endif
