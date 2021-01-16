#include <thread>
#include <atomic>
#include <chrono>
#include <tuple>
#include <iostream>
#include "barrier.h"
#include <vector>
#include <random>
#include "scq.h"
#include "Stats.h"

const long long OPS = 1000000;
const unsigned TIMES = 10;

template<class T>
void do_sth(T& Q, Barrier &bar, int l, unsigned loc_ops)
{
  bar.wait();
  for (int i = 0; i < loc_ops; ++i) {
      Q.dequeue();
    }
}

template<class T>
std::tuple<float,float> get_tp(int THREADS)
{
    float mopss[TIMES] = {0};

    for(int iteration = 0; iteration<TIMES; iteration++)
    {
      std::vector<std::thread> pool(THREADS);
      std::chrono::time_point<std::chrono::system_clock> start, end;

      double duration;

      T* Q = new T;
      //CODE TO BENCHMARK START

      Barrier bar(THREADS);

      for(int i =0; i<THREADS; i++)
        pool[i] = std::thread(do_sth<T>, std::ref(*Q), std::ref(bar), i, float(OPS)/THREADS);


      for(int i = 0; i < THREADS; i++)
      {
        pool[i].join();
      }

      end = std::chrono::system_clock::now();

      duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-bar.barrier_time).count()/float(1000000000);
      // duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()/float(1000000000);

      // std::cout << duration<< "\n";
      mopss[iteration] = (float(OPS)/duration)/1000000;
    }
    return calculate_stats(mopss, TIMES);
}

int main()
{
    std::cout << "\n1  " << std::get<0>(get_tp<scq<11>>(1)) << "ops/s\n";
    std::cout << "\n2  " << std::get<0>(get_tp<scq<11>>(2)) << "ops/s\n";
    std::cout << "\n4  " << std::get<0>(get_tp<scq<11>>(4)) << "ops/s\n";
    std::cout << "\n8  " << std::get<0>(get_tp<scq<11>>(8)) << "ops/s\n";
    std::cout << "\n16  " << std::get<0>(get_tp<scq<11>>(16)) << "ops/s\n";
    std::cout << "\n32  " << std::get<0>(get_tp<scq<11>>(32)) << "ops/s\n";
    std::cout << "\n64 " << std::get<0>(get_tp<scq<11>>(64)) << "ops/s\n";
}
