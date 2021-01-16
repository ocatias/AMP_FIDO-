#ifndef BARRIER
#define BARRIER

#include <atomic>
#include <mutex>

#include <cassert>
#include <condition_variable>


//This is my implementation of a Barrier
//It is inefficient (lots of atomic loads) and the comiler might simplify the loop away
//This is why I use the implementation below

// class Barrier
// {
// private:
//     std::atomic<unsigned> N;
//     std::atomic<unsigned> count;
// public:
//     Barrier(unsigned n)
//     {
//       N = n;
//     }
//
//     void wait()
//     {
//       count++;
//       while(count < N)
//       {
//          //Compiler might remove this loop, making it not an actual barrier
//       }
//
//     }
// };


#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>


//Using https://stackoverflow.com/questions/38999911/what-is-the-best-way-to-realize-a-synchronization-barrier-between-threads
//Its UmNyobe implementation

class Barrier
{

 public:
    Barrier(int count)
    {
      thread_count = count;
    }


    void wait()
    {
        //LOCK ME
        std::unique_lock<std::mutex> lk(mymutex);
        ++counter;
        if(counter < thread_count)
        {
          while(counter < thread_count)
          {
            cv.wait(lk);
          }
        }
        else{
          barrier_time = std::chrono::system_clock::now();
          cv.notify_all();
        }
        lk.unlock();
    }
    std::chrono::time_point<std::chrono::system_clock> barrier_time;
 private:
      std::mutex mymutex;
      std::condition_variable cv;
      int counter = 0;
      int waiting = 0;
      int thread_count;
};
#endif



// class Barrier
// {
//
//  public:
//     Barrier(int count)
//     {
//       thread_count = count;
//     }
//
//
//     void wait()
//     {
//         //LOCK ME
//         std::unique_lock<std::mutex> lk(mymutex);
//         ++counter;
//         ++waiting;
//         cv.wait(lk, [&]{return counter >= thread_count;});
//         cv.notify_one();
//         --waiting;
//         if(waiting == 0)
//         {
//            counter = 0;
//         }
//         lk.unlock();
//     }
//
//  private:
//       std::mutex mymutex;
//       std::condition_variable cv;
//       int counter = 0;
//       int waiting = 0;
//       int thread_count;
// };
