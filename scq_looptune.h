


#ifndef SCQ
#define SCQ

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>
#include <math.h>

unsigned long long pow(unsigned long long x, int power)
{
  unsigned long long y = 1;
  for(;power >= 1; power--)
  {
    y *= x;
  }
  return(y);
}

template<size_t bits, unsigned wait_loops>
class scq{
  public:
    scq(){
      int exponent = bits;

      if(exponent > 31)
        throw "Cannot assign this many bits to n. The maximum is 31 bits.";

      if(exponent < 0)
        throw "Need to assign a nonzero amount of bits to n.";

      int n = (pow(2, exponent));
      int N = (exponent);

      Tail.store(2*n);
      Head.store(2*n);
      Threshold.store(-1);
      Entries = (std::atomic_ullong*)std::malloc(2*n * sizeof(unsigned long long));
      for(int i = 0; i < 2*n; i++){
        Entries[i].store((2*n -1) + 2*n);
      }
    }
    ~scq()
    {
      free(Entries);
    }

    void enqueue(unsigned index){
      int N = bits;
      int n = (pow(2, N));
      if(index == -1 || index < 0 || index >= 2*n-1)
      {
        throw"Received a value that is in the wrong format.\n";
      }
      int pow2 = pow(2,N+1);

      while (true) {
        int T = std::atomic_fetch_add(&Tail, 1);
        // int j = T%(2*n);
        int j = cyclic_remap( T%(2*n));
        retry_enqueue:
        unsigned long long Ent = Entries[j].load();
        unsigned long long idx = Ent & (pow2-1);
        unsigned long long cycle = Ent >> (N+2);
        bool isSafe = (Ent >> N+1) & 1;

        if(cycle < (int)(T/(2*n)) && idx == (2*n-1) && (isSafe || Head.load() <= T))
        {
          unsigned long long New = ((int)(T/(2*n))<<(N+2)) + 1*pow2 + index;
          // std::cout << "Setting cycle " << ((int)(T/(2*n)))<< "\n";
          if(!Entries[j].compare_exchange_strong(Ent,New))
            goto retry_enqueue;
          if(Threshold.load() != 3*n -1)
            Threshold.store(3*n-1);
          return;
        }
      }
    }

    void catchup(int tail, int head){
      while(!Tail.compare_exchange_strong(tail, head))
      {
        head = Head.load();
        tail = Tail.load();
        if(tail >= head)
          break;
      }
    }

    unsigned dequeue(){
      if(Threshold.load() < 0)
      {
        //Queue is empty
        return(-1);
      }
      else{
        int N = bits;
        int n = (pow(2, N));
        int pow2 = pow(2,N+1);
        while (true) {
          int H = std::atomic_fetch_add(&Head, 1);
          // int j = H%(2*n);
          int j = cyclic_remap(H%(2*n));
          retry_dequeue:
          unsigned long long Ent = Entries[j].load();
          unsigned long long idx = Ent & (pow2-1);
          unsigned long long cycle = Ent >> (N+2);
          bool isSafe = (Ent >> N+1) & 1;

          if(cycle == (int)(H/(2*n)))
          {
            std::atomic_fetch_or(&Entries[j], (unsigned long long)(2*n-1));
            return(idx);
          }



          // unsigned long long New = ((int)(H/(2*n)+1)<<(N+2)) + idx;
          unsigned long long New = (cycle<<(N+2)) + idx;

          if(idx == pow2-1)
          {
            New = ((int)(H/(2*n))<<(N+2)) + isSafe*pow2 + 2*n-1;
          }

          if(cycle < (int)(H/(2*n)))
          {
            if(Tail.load() >= H+1)
            {
              int wait_int = 0;
              for(;;)
              {
                wait_int++;
                if(wait_int >= wait_loops)
                  break;
                //WE ARE TOO EARLY -> WAIT
              }
            }

            if(!Entries[j].compare_exchange_strong(Ent, New))
            {
              goto retry_dequeue;
            }

          }



          int T = Tail.load();

          if(T <= H+1)
          {
              catchup(T, H+1);
              std::atomic_fetch_add(&Threshold, -1);
              return(-1);
          }

          if(std::atomic_fetch_add(&Threshold,-1) <= 0)
          {
            return(-1);
          }

        }
      }
    }
  private:
    std::atomic<int> Tail, Head;
    std::atomic_ullong *Entries;
    std::atomic<int> Threshold;

    int cyclic_remap(int x)
    {
      int n = (pow(2, bits + 1));
      return (n/64*(x%64)+ floor(x/64));
      // return(x);
    }

    friend std::ostream& operator<<(std::ostream& os, const scq& me)
    {
      int N = bits;
      int n = (pow(2, N));
      os << "Cycle Head " << (int)(me.Head/(2*n)) << ", Cycle Tail " << (int)(me.Tail/(2*n)) <<  ", Tail " << me.Tail << ", Head " << me.Head << ", n " << n << ", Threshold " << me.Threshold << "\n";
      for(int i = 0; i < 32; i++)
      {
        unsigned long long Ent = me.Entries[i].load();
        int pow2 = pow(2,N+1);
        unsigned long long idx = Ent & (pow2-1);
        unsigned long long cycle = Ent >> (N+2);

        os  << std::bitset<64>(Ent) << " Idx: " << idx <<  ", cycle: " << cycle << "\n";
      }
      return(os);
    }
};


#endif
