//This is a strongly changed version of scq
//It counts how often certain loops get repeated
//DO NOT USE THIS FOR BENCHING THE SPEED
#ifndef SCQ
#define SCQ

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>
#include<tuple>
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

template<size_t bits>
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
    //Returns: 0:Loop repititions, 1:number of CAS fails, 2: number of times is_Unsafe
    int* enqueue(int index){
      int N = bits;
      int n = (pow(2, N));
      if(index == -1 || index < 0 || index >= 2*n-1)
      {
        throw"Received a value that is in the wrong format.\n";
      }

      static int returnval[3];
      int loops = 0;
      int cas_fails = 0;
      int is_unsafe = 0;

      int pow2 = pow(2,N+1);

      while (true) {
        int T = std::atomic_fetch_add(&Tail, 1);
        // int j = T%(2*n);
        int j = cyclic_remap( T%(2*n));
        retry_enqueue:
        loops++;
        unsigned long long Ent = Entries[j].load();
        unsigned long long idx = Ent & (pow2-1);
        unsigned long long cycle = Ent >> (N+2);
        bool isSafe = (Ent >> N+1) & 1;

        if(isSafe == false)
          is_unsafe++;

        if(cycle < (int)(T/(2*n)) && idx == (2*n-1) && (isSafe || Head.load() <= T))
          {
          unsigned long long New = ((int)(T/(2*n))<<(N+2)) + 1*pow2 + index;
          if(!Entries[j].compare_exchange_strong(Ent,New))
          {
            cas_fails++;
            goto retry_enqueue;
          }
          if(Threshold.load() != 3*n -1)
            Threshold.store(3*n-1);

          returnval[0] = loops;
          returnval[1] = cas_fails;
          returnval[2] = is_unsafe;
          return (returnval);
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
    //Returns: 0: dequeued index, 1: loop repitions, 2: accessing an old element, 3: cas fails, 4: calling catchup
    int* dequeue(){

      static int returnval[5];
      int loops = 0;
      int old_element = 0;
      int catchup_calls = 0;
      int cas_fails = 0;

      if(Threshold.load() < 0)
      {
        //Queue is empty
        returnval[0] = -1;
        returnval[1] = loops;
        returnval[2] = old_element;
        returnval[3] = cas_fails;
        returnval[4] = catchup_calls;
        return(returnval);
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
          loops++;
          unsigned long long Ent = Entries[j].load();
          unsigned long long idx = Ent & (pow2-1);
          unsigned long long cycle = Ent >> (N+2);
          bool isSafe = (Ent >> N+1) & 1;

          if(cycle == (int)(H/(2*n)))
          {
            std::atomic_fetch_or(&Entries[j], (unsigned long long)(2*n-1));
            returnval[0] = idx;
            returnval[1] = loops;
            returnval[2] = old_element;
            returnval[3] = cas_fails;
            returnval[4] = catchup_calls;
            return(returnval);
          }

          // unsigned long long New = {Ent.Cycle, 0, Ent.Index};
          unsigned long long New = (cycle<<(N+2)) + idx;

          if(idx == pow2-1)
          {
            New = ((int)(H/(2*n))<<(N+2)) + isSafe*pow2 + 2*n-1;
          }

          if(cycle < (int)(H/(2*n)))
          {
            old_element++;

            if(Tail.load() >= H+1)
            {
              int wait_int = 0;
              for(;;)
              {
                wait_int++;
                if(wait_int < 1000)
                  break;
                //WE ARE TOO EARLY -> WAIT
              }
            }

            if(!Entries[j].compare_exchange_strong(Ent, New))
            {
              cas_fails++;
              goto retry_dequeue;
            }
          }

          int T = Tail.load();

          if(T <= H+1)
          {
            catchup_calls++;
            catchup(T, H+1);
            std::atomic_fetch_add(&Threshold, -1);
            returnval[0] = -1;
            returnval[1] = loops;
            returnval[2] = old_element;
            returnval[3] = cas_fails;
            returnval[4] = catchup_calls;
            return(returnval);
          }

          if(std::atomic_fetch_add(&Threshold,-1) <= 0)
          {
            returnval[0] = -1;
            returnval[1] = loops;
            returnval[2] = old_element;
            returnval[3] = cas_fails;
            returnval[4] = catchup_calls;
            return(returnval);
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
      if(bits >= 5)
      {
        int n = (pow(2, bits + 1));
        return ((n/64)*(x%64)+ floor(x/64));
      }
      return(x);
    }

    friend std::ostream& operator<<(std::ostream& os, const scq& me)
    {
      int N = bits;
      int n = (pow(2, N));
      os << "Cycle Head " << (int)(me.Head/(2*n)) << ", Cycle Tail " << (int)(me.Tail/(2*n)) <<  ", Tail " << me.Tail << ", Head " << me.Head << ", n " << n << ", Threshold " << me.Threshold << "\n";
      for(int i = 0; i < 4; i++)
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
