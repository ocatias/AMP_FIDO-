#include <stdio.h>
#include "scq.h"
#include "scp.h"
#include "bllock2.h"
#include "bllock1.h"

#include "iostream"
#include <thread>
#include "barrier.h"
#include <cstdlib>
#include <vector>
// #include "infarrayQ.h"

const int THREADS_COUNT = 8;
thread_local const int INSERTS = 4000;

thread_local const int DISTANCE = 100*INSERTS;
const int TIMES = 10;

template<class T>
void thread_operation(T &Q, Barrier &bar, int thread_ID)
{
  bar.wait();
  for(int i = 1; i <= INSERTS; i++)
  {
    // std::cout << i;
    Q.enqueue(thread_ID*DISTANCE + i);
  }
}

template<class T>
void test()
{
  //BIG TEST RUN
  for(int times = 0; times < TIMES; times++)
  {
    T* Q = new T;
    std::cout << "\nTesting " << typeid(*Q).name() << "\n";
    std::cout << "Inserting " << THREADS_COUNT*INSERTS << " elements \n";



    int* Q_content = (int*)std::malloc(THREADS_COUNT*INSERTS * sizeof(int));

    std::vector<std::thread> pool(THREADS_COUNT);
    Barrier bar(THREADS_COUNT);
    //CODE TO BENCHMARK START

    for(int i = 0; i<THREADS_COUNT; i++)
      pool[i] = std::thread(thread_operation<T>, std::ref(*Q), std::ref(bar), i);


    for(auto & t : pool)
      t.join();

    for(int i = 0; i < THREADS_COUNT*INSERTS; i++)
    {
      Q_content[i] = Q->dequeue();
      // std::cout << Q_content[i] <<"\n";
    }


    //Check if all elements were enqueued in the correct order
    //I.e. that for no thread its i-th element is in front of the (i-1)-th element
    for(int i = 0; i < THREADS_COUNT; i++)
    {
      int c = -1;
      for (size_t j = 0; j < THREADS_COUNT*INSERTS; j++) {
        if(c == -1)
        {
          if(Q_content[j]/DISTANCE == i)
            c=Q_content[j];
        }
        else if(Q_content[j]/DISTANCE == i && Q_content[j] > c)
          c=Q_content[j];
        else if(Q_content[j]/DISTANCE == i && Q_content[j] <= c)
          std::cout << "ERROR:" << Q_content[j] << "   " << c << "\n";
      }
    }
    std::cout << "Checked " << THREADS_COUNT*INSERTS << " elements \n";
  }
}


int main(){
  test<bllock1<1048576>>();

  test<scq<25>>();
  test<scp<20>>();
  test<bllock2>();


}
