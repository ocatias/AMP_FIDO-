#include <stdio.h>
#include "ncq.h"
#include "scq.h"
#include "scp.h"
#include "iostream"
#include "bllock2.h"
#include "bllock1.h"

#include "barrier.h"
#include <thread>
#include <vector>
#include <cstdlib>
#include <stdexcept>

int THREADS_COUNT = 16;
long INSERTS = 1000000;
thread_local int OPS_PER_THREAD;

const int TIMES = 1;


template<class T>
void thread_operations(T &Q, Barrier &bar, int pid)
{
  bar.wait();

  OPS_PER_THREAD = INSERTS/THREADS_COUNT;
  for(long i = 1; i <= OPS_PER_THREAD; i++)
  {
    Q.enqueue(pid);
    int deq = Q.dequeue();
    if( deq == -1)
    {
      std::cout << Q;
      std::cout << Q;
      INSERTS = 0;
      break;
    }
  }
}

template<class T>
void test()
{
  for(int times = 0; times < TIMES; times++)
  {
    T* Q = new T;
    // std::cout << *Q;
    std::cout << "Testing " << typeid(*Q).name() << "\n";
    std::cout << "Inserting " << INSERTS << " elements \n";

    std::vector<std::thread> pool(THREADS_COUNT);
    Barrier bar(THREADS_COUNT);

    for(int i = 0; i<THREADS_COUNT; i++)
      pool[i] = std::thread(thread_operations<T>, std::ref(*Q), std::ref(bar), i);


    for(auto & t : pool)
      t.join();

    std::cout << "Checked " << INSERTS<< " elements \n";
    // std::cout << *Q;
    delete Q;
  }
}

int main(){

  //At this bit value SCQ does not use cyclic remapping
  test<scq<4>>();

  test<bllock1<1048576>>();
  test<scq<6>>();
  test<scp<6>>();
  test<bllock2>();
}
