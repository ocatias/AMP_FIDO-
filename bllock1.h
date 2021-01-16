#ifndef BLLOCK1
#define BLLOCK1

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>
// #include <recursive_mutex>
#include <mutex>
#include <condition_variable>
#include "Node.h"

template<long long capacity>
class bllock1{
  public:
    void enqueue(int x){
      // std::cout << "enqueue " << std::flush;
      bool wakeup = false;
      enqlock.lock();
      std::unique_lock<std::mutex> lk(mymutex);
      while (size.load() == capacity) {
        // std::cout << "FULL " << std::flush;
        notFull.wait(lk);
      }

      entries[tail%(capacity-1)] = x;
      tail++;

      unsigned curr_size = size.fetch_add(1);
      if(curr_size == 0)
        wakeup = true;

      enqlock.unlock();

      if(wakeup)
      {
        deqlock.lock();
        notEmpty.notify_all();
        deqlock.unlock();
      }
    }

    bllock1(){
      entries = (int*)std::malloc(capacity * sizeof(int));
      tail = 0;
      head = 0;
      size.store(0);

    }
    ~bllock1(){
      delete entries;
    }
    int dequeue(){
      // std::cout << "dequeue " << std::flush;
      int x;
      bool wakeup = false;
      std::unique_lock<std::mutex> lk(mymutex);
      while(size.load() == 0)
      {
        return(-1);
      }

      x = entries[head%(capacity-1)];
      head++;

      unsigned curr_size = size.fetch_add(-1);
      if(curr_size == capacity)
        wakeup = true;

      if(wakeup)
      {
        enqlock.lock();
        notFull.notify_all();
        enqlock.unlock();
      }
      return(x);
    }
  private:
    std::recursive_mutex enqlock, deqlock;
    std::condition_variable notEmpty, notFull;
    std::atomic<long> size;
    unsigned long long tail, head;
    int* entries;

    std::mutex mymutex;

    friend std::ostream& operator<<(std::ostream& os, const bllock1& me)
    {

      return(os);
    }
};

#endif
