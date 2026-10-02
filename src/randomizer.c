#include "loader.h"
#include "stdlib.h"
#include "time.h"

long xoshiroSeed(long seed) {
  long s = seed;
  s ^= s << 12;
  s ^= s >> 7;
  s ^= s << 9;
  s = (s << 23) | (s >> (sizeof s - 23));

  return s;
}

int xoshiroCielInt(long seed, int ciel) {
  long s = xoshiroSeed(seed);
  
  return labs((s * 10345345 + 1823535) % ciel) ;
};

long timeSeed(void) {
  time_t seed = time(0);
  return seed;
}
