#ifndef STATS
#define STATS

#include <stdio.h>
#include <tuple>
#include <math.h>

std::tuple<float,float> calculate_stats(float *array, int size)
{
  float mean = 0;
  for(int i = 0; i < size; i++)
    mean += array[i];

  mean /= size;
  float sd = 0;
  for(int i = 0; i < size; i++)
  {
    sd += (mean-array[i])*(mean-array[i]);
  }
  sd = sqrt(sd);
  sd /= (size-1);

  return std::make_tuple(mean,sd);
}


#endif
