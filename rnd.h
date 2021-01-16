#ifndef RND
#define RND


//Simple Linear congruential generator
class rnd{
public:
  rnd(unsigned seed)
  {
    X = seed;
  }
  unsigned get_rand()
  {
    X =(a*X + c) % m;
    return(X);
  }
private:
  unsigned X;

  //parameters Pierre L'ecuyer's "Table Of Linear Contrentual Generators Of Different Sizes  And Good Lattice Structures"
  int a = 438293613;
  int c = 3;
  int m = 1073741824;
};


#endif
