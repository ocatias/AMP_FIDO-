#include <stdio.h>
#include "scq.h"
#include "scp.h"
#include "bllock2.h"
#include "bllock1.h"

#include "iostream"
#include <cstdlib>
#include <bitset>

const int THREADS_COUNT = 2;
const int INSERTS = 100000;

const int TIMES = 100;

unsigned long long poww(unsigned long long x, int power)
{
  unsigned long long y = 1;
  for(;power >= 1; power--)
  {
    y *= x;
  }
  return(y);
}

int main(){
  // scq Q;

  bllock1<8> Q;

  Q.enqueue(1);
  std::cout << Q.dequeue();
  // std::cout << "Init\n";
  //
  // unsigned long long Ent = (poww(2,17)-1);
  // std::bitset<64> y(Ent);
  //
  // std::cout << "\n" << y;
  // Ent += poww(2,19)+poww(2,20);
  //
  // std::bitset<64> y2(Ent);
  // std::cout << "\n" << y2;
  //
  // unsigned long long idx = (Ent & (poww(2,17)-1));
  // std::bitset<64> y3(idx);
  // std::cout << "\n" << y3;
  //
  // std::cout << "\n\n\n";
  // std::cout << Q;
  //
  // #pragma omp parallel num_threads(THREADS_COUNT)
  // {
  //   int thread_ID = omp_get_thread_num();
  //   for(int i = 0; i < 5; i++)
  //     Q.enqueue(100*thread_ID+i);
  // }

  // for(int i = 0; i < 100; i++)
  // {
  //   Q.enqueue(1);
  //   Q.enqueue(1);
  //  Q.dequeue();
  //   Q.dequeue();
  //   std::cout << Q;
  //
  // }
  Q.enqueue(1);
  Q.enqueue(2);
  Q.enqueue(3);
  Q.enqueue(4);
  Q.enqueue(5);
  Q.enqueue(6);
  //
  //
  // std::cout << Q;
  // // std::cout << Q;
  //
  std::cout << "NOW STARTING TO DEQUEUEUE\n\n\n";
  for(int i = 0; i < 7; i++)
  {
    std::cout << Q.dequeue() << "\n";
    std::cout << Q;
  }

  // std::cout << Q;
  // std::cout << "End\n";
  // std::cout << Q.dequeue() << "\n"
  // << Q;
  // std::cout << Q.dequeue() << "\n"
  // << Q;
}
