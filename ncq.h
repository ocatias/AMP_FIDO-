#include <atomic>
#include <cstdlib>
#include <iostream>
#include <bitset>

struct Entry
{
  int Cycle, Index;
};

class ncq
{
  public:
    ncq()
    {

      Tail = n;
      Head = n;
      Entries = (std::atomic<struct Entry>*)std::malloc(n * sizeof(Entry));
      for(int i = 0; i < n; i++){
        Entries[i] = {0,0};
      }
    }
    ~ncq()
    {
      free(Entries);
    }
    void enqueue(int index)
    {
      long long T, j;
      struct Entry New, Ent;
      do {
        aa12:

        T = Tail.load();
        j = T%n;
        Ent = Entries[j].load();
        if(Ent.Cycle == (int)(T/n))
        {
          Tail.compare_exchange_strong(T,T+1);
          goto aa12;
        }

        if(Ent.Cycle + 1 != (T/n))
          goto aa12;

        New = {static_cast<int>(T/n), index};
      } while(!Entries[j].compare_exchange_strong(Ent,New));
      // std::cout << "...\n";
      Tail.compare_exchange_strong(T, T+1);


    }
    int dequeue()
    {
      long long H, j;
      struct Entry Ent;
      do {
        aa77:

        H = Head.load();
        j = H%n;
        Ent = Entries[j].load();
        if(Ent.Cycle != H/n)
        {
          if(Ent.Cycle + 1 == H/n)
          {
            return(-1);
          }
          // std::cout << Ent.Cycle << ", H " << H << ",n " << n <<"\n";
          goto aa77;
        }
      } while(! Head.compare_exchange_strong(H,H+1));
      return Ent.Index;
    }

    friend std::ostream& operator<<(std::ostream& os, const ncq& me)
    {
      long n = me.n;
      os << "Tail " << me.Tail << ", Head " << me.Head << ", n " << n << "\n";
      for(int i = 0; i < n; i++)
      {
        struct Entry Ent = me.Entries[i].load();
        os << "(Cycle " << Ent.Cycle << ", Index " << Ent.Index << ")" << "\n";
      }
      return(os);
    }
  private:
    std::atomic<long long> Tail, Head;
    std::atomic<struct Entry> *Entries;
    long long n = 1000000;

};
