#ifndef SCP
#define SCP

#include "scq.h"
#include <math.h>

template<size_t bits>
class scp{
  public:
    scp()
    {
      // std::cout << "INITIALIZE SCP\n";
      long unsigned n = pow(2,bits);
      data = (unsigned*)std::malloc(n * sizeof(unsigned));

      aq = new scq<bits>;
      fq = new scq<bits>;

      for(long unsigned i = 0; i < n; i++)
      {
        fq->enqueue(i);
      }
    }

    bool enqueue(int x)
    {
      unsigned index = fq->dequeue();
      if(index == -1)
      {
        // std::cout << "full";
        return(false);
      }
      data[index] = x;
      aq->enqueue(index);
      return(true);
    }

    ~scp()
    {
      free(data);
      fq->~scq();
      aq->~scq();
    }
    int dequeue()
    {
      unsigned index = aq->dequeue();
      if(index == -1)
      {
        // std::cout << "empty";
        return(-1);
      }
      unsigned returnval = data[index];
      // std::cout << "Ret " << index << ",";
      fq->enqueue(index);
      return(returnval);
    }
  private:
    scq<bits> *aq,*fq;
    unsigned *data;

    friend std::ostream& operator<<(std::ostream& os, const scp& me)
    {
      os << "AQ:\n" << (*me.aq) << "\nFQ:\n" << (*me.fq);

      return(os);
    }
};





#endif
