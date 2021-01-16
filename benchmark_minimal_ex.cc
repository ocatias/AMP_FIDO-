#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include "barrier.h"
#include <vector>
#include <random>
#include "scq.h"

const long long OPS = 1000000;

template<class T>
void do_sth(T& Q, Barrier &bar, int l, unsigned loc_ops)
{
  // std::cout << loc_ops << "\n";
  // bar.wait();

  std::mt19937 r;
  std::uniform_real_distribution<double> dist{0, 1};

  bar.wait();
  for (int i = 0; i < loc_ops; ++i) {
      // Q.enqueue(i%2048);
      Q.dequeue();
    }
}

template<class T>
double get_tp(int THREADS)
{
    double threadtime[THREADS] = {0};

    double total_time = 0;

    std::vector<std::thread> pool(THREADS);
    std::chrono::time_point<std::chrono::system_clock> start, end;

    double duration;

    T* Q = new T;
    //CODE TO BENCHMARK START

    start = std::chrono::system_clock::now();
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
    double mopss = (float(OPS)/duration)/1000000;

    return mopss;
}

int main()
{
    std::cout << "\n1  " << get_tp<scq<11>>(1) << "ops/s\n";
    std::cout << "\n2  " << get_tp<scq<11>>(2) << "ops/s\n";
    std::cout << "\n4  " << get_tp<scq<11>>(4) << "ops/s\n";
    std::cout << "\n8  " << get_tp<scq<11>>(8) << "ops/s\n";
    std::cout << "\n16  " << get_tp<scq<11>>(16) << "ops/s\n";
    std::cout << "\n32  " << get_tp<scq<11>>(32) << "ops/s\n";
    std::cout << "\n64 " << get_tp<scq<11>>(64) << "ops/s\n";
}
