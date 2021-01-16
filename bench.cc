#include <stdio.h>
#include "scq.h"
#include "scp.h"
// #include "infarrayQ.h"
#include "bllock1.h"
#include "bllock2.h"
#include "rnd.h"
#include <iostream>
#include <fstream>
#include "barrier.h"
#include <thread>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>
#include <functional>
#include <stdlib.h>
#include <math.h>
#include "Stats.h"
#include <tuple>

const unsigned OPS = 500000;
const int TIMES = 35;
const unsigned bits = 11;
const unsigned size = 4096;

long maxsize = 0;
float probability_enqueue = 0.5;

bool prefill = false;
float prefill_percentage = 0;

template<class T>
void do_prefill(T* Q)
{
  // std::cout << "prefill " << 65536*prefill_percentage << "\n";
  if (prefill) {
    int prefill_number = maxsize*prefill_percentage;
    // std::cout << "Prefilling with " << prefill_number << " elements\n";
    for(int i = 0; i < (prefill_number);i++)
      Q->enqueue(i);
  }
}

template<class T>
void pair_thread(T &Q, Barrier &bar, int OPS_PER_THREAD, int id)
{

  long loc_maxsize = maxsize;
  bar.wait();

  for(int j = 1; j <= OPS_PER_THREAD/2; j++)
  {
    Q.enqueue(j%(loc_maxsize-1));
    Q.dequeue();
  }
}


template<class T>
void enqdeq_thread(T &Q, Barrier &bar, int OPS_PER_THREAD, int id)
{
  float loc_probability_enqueue = probability_enqueue;

  long loc_maxsize = maxsize;
  rnd RNG(id);
  int enq = 0;

  bool do_enqueue[OPS_PER_THREAD];
  for(unsigned j = 1; j <= OPS_PER_THREAD; j++)
  {
      if(RNG.get_rand()%100 < (loc_probability_enqueue*100))
      {
        do_enqueue[j] = true;
      }
      else
        do_enqueue[j] = false;
  }

  // std::cout << enq/float(OPS_PER_THREAD) << "\n";
  bar.wait();

  for(int j = 1; j <= OPS_PER_THREAD; j++)
  {
    // if(j==142)
    // {
    //
    //   std::cout << Q  << std::flush;
    // }
    if(do_enqueue[j])
    {
      enq++;

      Q.enqueue(j%(loc_maxsize-1));
    }
    else{
      Q.dequeue();
    }
    // if(j%500 == 0)
      // std::cout << j << "\n" << std::flush;

  }
}

template<class T>
std::tuple<float, float> test_enqdeq(int _threads, int state, int _ops, int times)
{
  float MOPS = _ops/(1000000.);

  float total_time[times];
  for(int iteration = 0; iteration < times; iteration++)
  {
    T* Q = new T;
    do_prefill<T>(Q);

    std::vector<std::thread> pool(_threads);
    std::chrono::time_point<std::chrono::system_clock> start, end;

    // start = std::chrono::system_clock::now();

    Barrier bar(_threads);
    //CODE TO BENCHMARK START
    if(state == 0)
    {
      for(int i =0; i<_threads; i++)
        pool[i] = std::thread(enqdeq_thread<T>, std::ref(*Q), std::ref(bar), _ops/_threads, i);
    }
    else if(state == 1)
    {
      for(int i =0; i<_threads; i++)
        pool[i] = std::thread(pair_thread<T>, std::ref(*Q), std::ref(bar), _ops/_threads, i);
    }
    else
      throw"Invalid state";

    for(auto & t : pool)
      t.join();

    end = std::chrono::system_clock::now();
    double elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end-bar.barrier_time).count();
    total_time[iteration] = elapsed_ns/1000000000;
    // std::cout << "deleting now " << std::flush;
    delete Q;
    // std::cout << " deleted\n" << std::flush;
  }

  float all_mopss[times];
  for(int k = 0; k < times; k++)
  {
    all_mopss[k] = _ops/1000000./total_time[k];
    // std::cout << all_mopss[k] << "Mops/s\n";
  }


  std::tuple<float,float> results = calculate_stats(all_mopss, times);

  return(results);
}


void test(int argc, char *argv[])
{
  maxsize = pow(2,bits);
  int threads[8] = {1,2,4,8,16,32,64,128};
  int state = 0;
  if(argc < 2)
    throw "Please use proper prgram input:\n1: Mode (pairs, enqdeq)\n2: Enqueue Probability\n3: Prefill Percentage\n";

  if(argc >= 3)
  {
    probability_enqueue = atof(argv[2]);
    if(probability_enqueue > 0.5)
    {
      std::cout << "Enqueue probability is > 0.5, this will most likely overflow some queues.\n";
    }
    if(probability_enqueue > 1 || probability_enqueue < 0)
    {
      throw "Invalid enqueue probability\n";
    }
  }

  std::cout << "Allocating " << bits << " bits for SCQ.\n";
  std::string mode = std::string(argv[1]);

  std::function<std::tuple<float,float>(int,int,int,int)> scq_test = test_enqdeq<scq<bits>>;
  std::function<std::tuple<float,float>(int,int,int,int)> scp_test = test_enqdeq<scp<bits>>;
  std::function<std::tuple<float,float>(int,int,int,int)> bllock1_test = test_enqdeq<bllock1<size>>;
  std::function<std::tuple<float,float>(int,int,int,int)> bllock2_test = test_enqdeq<bllock2>;

  if(mode == "pairs"){
    state = 1;

    std::cout << "Testing enqueue dequeue pairs\n";
  }
  else if(mode == "enqdeq")
  {
    state = 0;

    std::cout << "Testing enqueue dequeue with enqueue probability ";
    std::cout << probability_enqueue << "\n";
  }
  else{
    throw "Invalid mode.\n";
  }

  if(argc >= 4)
  {
    prefill_percentage = atof(argv[3]);
    if(prefill_percentage > 0)
      prefill = true;
    if ( prefill_percentage < 0) {
      throw "Invalid prefill percentage";
    }
  }
  std::cout << "Prefilling the queues with " << prefill_percentage*100 << "%\n";

  std::string filename = "./Results/" + mode + "_" + std::to_string(probability_enqueue).substr(0,4) + "_" + std::to_string(prefill_percentage).substr(0,4) + ".txt";
  std::ofstream file;
  file.open(filename);
  std::cout << "Writing results to " << filename << "\n";

  file << "Threads, SCQ, SCQ_sd, SCP, SCP_sd, BLQ1, BLQ1_sd, BLQ2, BLQ2_sd\n";
  for(int i = 0; i < 8; i++)
  {
    std::cout << threads[i] << " threads\n";

    //SCQ
    std::tuple<float, float> scq_mopss = scq_test(threads[i], state, OPS, TIMES);
    std::cout << "   SCQ " << std::get<0>(scq_mopss) << "± " << std::get<1>(scq_mopss) << " Mops/sec\n";

    //SCP
    std::tuple<float, float> scp_mopss = scp_test(threads[i], state, OPS, TIMES);
    std::cout << "   SCP " << std::get<0>(scp_mopss) << "± " << std::get<1>(scp_mopss) << " Mops/sec\n";

    //BLLOCK1
    std::tuple<float, float> bllock1_mopss = bllock1_test(threads[i], state, OPS, TIMES);
    std::cout << "   BLL1 " << std::get<0>(bllock1_mopss) << "± " << std::get<1>(bllock1_mopss) << " Mops/sec\n";


    //BLLOCK2
    std::tuple<float, float> bllock2_mopss = bllock2_test(threads[i], state, OPS, TIMES);
    std::cout << "   BLL2 " << std::get<0>(bllock2_mopss) << "± " << std::get<1>(bllock2_mopss) << " Mops/sec\n";
    std::cout << "\n";

    file << threads[i] << "," << std::get<0>(scq_mopss) << "," << std::get<1>(scq_mopss)
    << ", " << std::get<0>(scp_mopss) << "," << std::get<1>(scp_mopss)
    << ", " << std::get<0>(bllock1_mopss) << "," << std::get<1>(bllock1_mopss)
    << ", " << std::get<0>(bllock2_mopss) << "," << std::get<1>(bllock2_mopss)
    << "\n";
  }
  file.close();
}

int main(int argc, char *argv[]){
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

    test(argc, argv);

  end = std::chrono::system_clock::now();

  double runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std::cout << "Total runtime " << (runtime_ms/1000) << "s\n";
}
