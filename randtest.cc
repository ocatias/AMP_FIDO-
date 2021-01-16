#include <stdio.h>
#include "iostream"
#include <omp.h>
#include <cstdlib>
#include <bitset>
#include<tuple>
#include <chrono>

//Simple Linear congruential generator
class rnd{
public:
  rnd(int seed)
  {
    X = seed;
  }
  unsigned get_rand()
  {
    X =(a*X + c) % m;
    return(X);
  }
private:
  int X;

  //parameters Pierre L'ecuyer's "Table Of Linear Contrentual Generators Of Different Sizes  And Good Lattice Structures"

  int a = 438293613;
  int c = 3;
  int m = 1073741824;
};


int main(){
  rnd RNG(3);
  int count = 0;
  int tries = 1000000000;
  for(int i = 0; i < tries; i++)
  {
    if(RNG.get_rand()%100 < (0.5*100))
      count++;
  }
  std::cout << float(count)/tries;
}
