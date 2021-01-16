#include <stdio.h>
#include "scq_bench.h"
#include "rnd.h"
#include "barrier.h"
#include "Stats.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <bitset>
#include <tuple>
#include <chrono>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <math.h>

const int OPS = 500000;
const int TIMES = 35;
const unsigned bits = 11;

float prefill_percentage = 0;
bool use_pairs = false;
template<class T>
void do_prefill(T* Q)
{
  unsigned max_elements = pow(2, bits);
  if (prefill_percentage >= 0) {
    for(int i = 0; i < (max_elements*prefill_percentage);i++)
      Q->enqueue(i);
  }
}

template<class T>
void enqdeq_thread(T &Q, Barrier &bar1, Barrier &bar2, bool use_pairs, int THREADS_COUNT, float loc_probability_enqueue , int thread_ID, float *stats_enqueue_glbl, float *stats_dequeue_glbl)
{
    int enqueues = 0;
    int dequeues = 0;
    float stats_enqueue[3] = {0,0,0};

    float OPS_PER_THREAD = float(OPS)/THREADS_COUNT;

    float stats_dequeue[4] = {0,0,0,0};

    rnd RNG(0 + thread_ID -1);

    unsigned max_elements = pow(2, bits);

    bar1.wait();
    if(use_pairs)
    {
      for(int i = 1; i <= OPS_PER_THREAD/2; i++)
      {
          enqueues++;
          dequeues++;
          int* stats_en = Q.enqueue(i%(max_elements-1));

          //NOTE: the 0th value returned by Q.dequeue() is the index
          int* stats_de = Q.dequeue();

          for(int k = 0; k < 4; k++)
            stats_dequeue[k] += stats_de[k+1];

          for(int k = 0; k < 3; k++)
              stats_enqueue[k] += stats_en[k];
      }
    }
    else
    {
      for(int i = 1; i <= OPS_PER_THREAD; i++)
      {
        if(RNG.get_rand()%100 < (loc_probability_enqueue*100))
        {
          enqueues++;
          int* stats = Q.enqueue(i%(max_elements-1));
          for(int k = 0; k < 3; k++)
            {
              stats_enqueue[k] += stats[k];
            }
        }
        //Dequeue
        else{
          dequeues++;
          //NOTE: the 0th value returned by Q.dequeue() is the index
          int* stats = Q.dequeue();
          for(int k = 0; k < 4; k++)
          {
            stats_dequeue[k] += stats[k+1];
          }
        }
      }
    }


    //Wait for all other threads before continuing
    bar2.wait();

    for(int k = 0; k < 3; k++)
    {
      stats_enqueue[k] /= float(enqueues);
      stats_enqueue_glbl[k] = stats_enqueue[k];
    }

    for(int k = 0; k < 4; k++)
    {
      stats_dequeue[k] /= float(dequeues);
      stats_dequeue_glbl[k] = stats_dequeue[k];
    }
}

template<class T>
std::tuple<std::tuple<float,float,float,float,float,float,float>, std::tuple<float,float,float,float,float,float,float>> test_run(float probability_enqueue = 0.5, int THREADS_COUNT = 2)
{
  if(use_pairs)
    std::cout << "Doing a run with " << THREADS_COUNT << " threads, using pairs\n";
  else
    std::cout << "Doing a run with " << THREADS_COUNT << " threads, enqueue probability = " << probability_enqueue << "\n";


  float stats_enqueue_thread[THREADS_COUNT][TIMES][3];
  float stats_dequeue_thread[THREADS_COUNT][TIMES][4];

  for(int x = 0; x < THREADS_COUNT; x++)
  {
    for(int y = 0; y < TIMES; y++)
    {
      for(int z = 0; z  < 3; z++)
      {
        stats_enqueue_thread[x][y][z] = 0;
        stats_dequeue_thread[x][y][z] = 0;
      }
      stats_dequeue_thread[x][y][3] = 0;
    }
  }
  float stats_enqueue_total[TIMES][3];
  float stats_dequeue_total[TIMES][4];
  for(int x = 0; x < TIMES; x++)
  {
    for(int z = 0; z  < 3; z++)
    {
      stats_enqueue_total[x][z] = 0;
      stats_dequeue_total[x][z] = 0;
    }
    stats_dequeue_total[x][3] = 0;
  }
  float stats_enqueue_final[3];
  float stats_dequeue_final[4];
  float stats_enqueue_final_sd[3];
  float stats_dequeue_final_sd[4];
  for(int z = 0; z  < 3; z++)
  {
    stats_enqueue_final[z] = 0;
    stats_dequeue_final[z] = 0;
    stats_enqueue_final_sd[z] = 0;
    stats_dequeue_final_sd[z] = 0;
  }
  stats_dequeue_final[3] = 0;
  stats_dequeue_final_sd[3] = 0;


  for(int iteration = 0; iteration < TIMES; iteration++)
  {
    T Q;

    do_prefill<T>(&Q);
    //Initialize Threads

    Barrier bar1(THREADS_COUNT);
    Barrier bar2(THREADS_COUNT);

    std::vector<std::thread> pool(THREADS_COUNT);

    for(int i =0; i<THREADS_COUNT; i++)
      pool[i] = std::thread(enqdeq_thread<T>, std::ref(Q), std::ref(bar1), std::ref(bar2), use_pairs, THREADS_COUNT, probability_enqueue, i,  stats_enqueue_thread[i][iteration], stats_dequeue_thread[i][iteration]);

    for(auto & t : pool)
      t.join();
  }

  //Update global threads
  for(int iteration = 0; iteration < TIMES; iteration++)
  {
    for(int thread = 0; thread < THREADS_COUNT; thread++)
    {
      for(int k = 0; k < 3; k++)
      {
          stats_enqueue_total[iteration][k] += stats_enqueue_thread[thread][iteration][k];
      }

      for(int k = 0; k < 4; k++)
      {
        stats_dequeue_total[iteration][k] += stats_dequeue_thread[thread][iteration][k];
      }

    }

    for(int k = 0; k< 3; k++)
    {
      stats_enqueue_total[iteration][k]  /= THREADS_COUNT;
      stats_dequeue_total[iteration][k] /= THREADS_COUNT;
    }

    //Dequeue has one more stat
    stats_dequeue_total[iteration][3] /= THREADS_COUNT;
  }

  for(int k = 0; k < 4; k++)
  {
    float stat_en[TIMES];
    float stat_de[TIMES];
    for(int iteration = 0; iteration < TIMES; iteration++)
    {
      stat_de[iteration] = stats_dequeue_total[iteration][k];
      if(k != 3)
        stat_en[iteration] = stats_enqueue_total[iteration][k];
    }

    stats_dequeue_final[k] = std::get<0>(calculate_stats(stat_de, TIMES));
    stats_dequeue_final_sd[k] = std::get<1>(calculate_stats(stat_de, TIMES));
    if(k != 3)
    {
      stats_enqueue_final[k] = std::get<0>(calculate_stats(stat_en, TIMES));
      stats_enqueue_final_sd[k] = std::get<1>(calculate_stats(stat_en, TIMES));
    }
  }


      std::cout << "  ENQUEUE Loops: " << stats_enqueue_final[0] << ",  CAS fails: "
        << stats_enqueue_final[1] << ", isUnsafe: " << stats_enqueue_final[2] << "\n";
        std::cout << "  DEQUEUE Loops: " << stats_dequeue_final[0] << ", Old Element access " << stats_dequeue_final[1]
         << ",  CAS fails: "  << stats_dequeue_final[2] << ", Catchup Calls: " << stats_dequeue_final[3] << "\n";

    std::cout << "  ENQ_SD Loops: " << stats_enqueue_final_sd[0] << ",  CAS fails: "
       << stats_enqueue_final_sd[1] << ", isUnsafe: " << stats_enqueue_final_sd[2] << "\n";
       std::cout << "  DEQ_SD Loops: " << stats_dequeue_final_sd[0] << ", Old Element access " << stats_dequeue_final_sd[1]
        << ",  CAS fails: "  << stats_dequeue_final_sd[2] << ", Catchup Calls: " << stats_dequeue_final_sd[3] << "\n";

    return std::make_tuple(std::make_tuple(stats_enqueue_final[0], stats_enqueue_final[1],
      stats_enqueue_final[2], stats_dequeue_final[0], stats_dequeue_final[1],
      stats_dequeue_final[2], stats_dequeue_final[3]),
      std::make_tuple(stats_enqueue_final_sd[0], stats_enqueue_final_sd[1],
        stats_enqueue_final_sd[2], stats_dequeue_final_sd[0], stats_dequeue_final_sd[1],
        stats_dequeue_final_sd[2], stats_dequeue_final_sd[3]));
}


template<class T>
void test()
{
  int threads[] = {1,2,4,8, 16, 32, 64, 128};
  float probabilities[] = {0, 0.01, 0.5};


  for(int idx_probability = 0; idx_probability < 2; idx_probability++)
  {

    std::string filename;
    if(use_pairs)
      filename = "./Results/scq_diss_pairs_" + std::to_string(prefill_percentage).substr(0,4) + ".txt";
    else
      filename = "./Results/scq_diss_enqdeq_" + std::to_string(probabilities[idx_probability]).substr(0,4) + "_" + std::to_string(prefill_percentage).substr(0,4) + ".txt";
    std::cout << "Saving results to " << filename << "\n";

    std::ofstream file;
    file.open(filename);
    file << "Threads, Enq_loops, Enq_CAS_fails, ENQ_unSafe, Deq_loops, Deq_old_element, Deq_CAS_fail, Deq_catchup_calls\n";

    for(int idx_thread = 0; idx_thread < 8; idx_thread++)
    {
      file << threads[idx_thread] << ", ";
      auto tuple2 = test_run<scq<bits>>(probabilities[idx_probability], threads[idx_thread]);
      auto tuple1_1 = std::get<0>(tuple2);
      auto tuple1_2 = std::get<1>(tuple2);
      file << std::get<0>(tuple1_1) << ", " << std::get<1>(tuple1_1) << ", " << std::get<2>(tuple1_1) << ", " << std::get<3>(tuple1_1) << ", "
        << std::get<4>(tuple1_1) << ", " << std::get<5>(tuple1_1) << ", " << std::get<6>(tuple1_1) << ", ";
      file << std::get<0>(tuple1_2) << ", " << std::get<1>(tuple1_2) << ", " << std::get<2>(tuple1_2) << ", " << std::get<3>(tuple1_2) << ", "
        << std::get<4>(tuple1_2) << ", " << std::get<5>(tuple1_2) << ", " << std::get<6>(tuple1_2) << "\n";
    }

    if(use_pairs)
      break;
  }
}

//1: usepairs, 2: Prefill percentage
int main(int argc, char *argv[]){
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  if(argc < 3)
    throw "Please use proper prgram input:\n1: Use pairs 'pairs', enqdeq 'enqdeq' 0\n2: Prefill Percentage\n";

  std::cout << "Using " << bits << " bits for SCQ.\n";

  prefill_percentage = atof(argv[2]);

  if(std::string(argv[1]) == "pairs")
    use_pairs = true;
  else
    use_pairs = false;

  if (prefill_percentage > 1 || prefill_percentage < 0) {
    throw "Invalid prefill percentage";
  }
  std::cout << "Using a prefill percentage of " << prefill_percentage*100 << "%\n";

  test<scq<bits>>();

  end = std::chrono::system_clock::now();
  double elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std::cout << "Time to run: " << elapsed_time_ms/1000 << "s\n";
}
