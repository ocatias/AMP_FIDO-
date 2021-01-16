#include <stdio.h>
#include "scq.h"
#include "scp.h"

#include "bllock2.h"
#include "bllock1.h"

#include "rnd.h"
#include "barrier.h"

#include "iostream"
#include <thread>
#include <cstdlib>
#include <vector>
#include <algorithm>


const int THREADS_COUNT = 40;
const int INSERTS = 5000;

const int DISTANCE = 100*INSERTS;
const int TIMES = 3;

template<class T>
void operation_thread(T &Q, Barrier &bar, int thread_ID, int times, std::vector<int> &vec_in_glob, std::vector<int> &vec_out_glob)
{
  std::vector<int> vec_in;
  std::vector<int> vec_out;

  int loc_inserts = INSERTS;
  int loc_distance = DISTANCE;
  rnd rng(thread_ID +13+(times+1)*107);


  bar.wait();
  for(int i = 1; i <= loc_inserts; i++)
  {
    if(rng.get_rand() % 2 == 0)
    {
      Q.enqueue(thread_ID + i);
      vec_in.push_back(thread_ID + i);
    }
    else{
      int out = Q.dequeue();
      if(out != -1)
        vec_out.push_back(out);
    }
  }
  vec_in_glob = vec_in;
  vec_out_glob = vec_out;
}

template<class T>
void test_queue()
{
  //BIG TEST RUN
  for(int times = 0; times < TIMES; times++)
  {
    T Q;
    std::cout << "\nTesting " << typeid(Q).name() << "\n";
    std::cout << "Inserting " << THREADS_COUNT*INSERTS << " elements \n";

    std::vector<int> vecs_in[THREADS_COUNT];
    std::vector<int> vecs_out[THREADS_COUNT];

    std::vector<std::thread> pool(THREADS_COUNT);

    Barrier bar(THREADS_COUNT);
    //CODE TO BENCHMARK START

    for(int i = 0; i<THREADS_COUNT; i++)
      pool[i] = std::thread(operation_thread<T>, std::ref(Q), std::ref(bar), i, times, std::ref(vecs_in[i]), std::ref(vecs_out[i]));


    for(auto & t : pool)
      t.join();


    //Collect elements in a single vector
    std::vector<int> vec_all_elements;
    std::vector<int> vec_ellements_from_Q;
    for(int i = 0; i < THREADS_COUNT; i++)
    {
      vec_all_elements.insert(vec_all_elements.end(), vecs_in[i].begin(), vecs_in[i].end());
      vec_ellements_from_Q.insert(vec_ellements_from_Q.end(), vecs_out[i].begin(), vecs_out[i].end());
    }

    // std::cout << "Should be:\n";
    // for (auto i = vec_all_elements.begin(); i != vec_all_elements.end(); ++i)
    //   std::cout << *i << ' ';
    //
    // std::cout << "\nAlready dequeued:\n";
    // for (auto i = vec_ellements_from_Q.begin(); i != vec_ellements_from_Q.end(); ++i)
    //     std::cout << *i << ' ';
    //
    // std::cout << "\n Q:\n" << Q << "\n";

    for(int i = 0; i < THREADS_COUNT*INSERTS; i++)
    {
      int ell = Q.dequeue();
      if(ell != -1)
        vec_ellements_from_Q.push_back(ell);
    }

    std::cout << "Checking " << THREADS_COUNT*INSERTS << " elements \n";

    if(vec_all_elements.size() != vec_ellements_from_Q.size())
      std::cout << "Number of elements in queue is wrong. (Q: " << vec_ellements_from_Q.size() << ", should be: " << vec_all_elements.size() << ")\n";

    //This is pretty slow, could be speed up by using a hashtable
    while(vec_ellements_from_Q.size() != 0)
    {
      int ell = vec_ellements_from_Q.back();
      vec_ellements_from_Q.pop_back();
      vec_all_elements.erase(std::remove(vec_all_elements.begin(), vec_all_elements.end(), ell), vec_all_elements.end());
    }

    if(vec_all_elements.size() != vec_ellements_from_Q.size())
      std::cout << "There are different elements in Q and the vector\n";

    std::cout << "Checked\n";
  }
}

int main(){
  std::cout << "Testing whether the queue looses any elements \n"
    << "Threads: " << THREADS_COUNT << ", Inserts/Thread: " << INSERTS
    << ", Times: " << TIMES << "\n";

    test_queue<bllock1<1048576>>();

    test_queue<bllock2>();

    test_queue<scp<20>>();
    test_queue<scq<20>>();
    // test_queue<ncq>();

  }
