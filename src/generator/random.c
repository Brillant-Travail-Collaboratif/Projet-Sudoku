#include "random_internal.h"

uint32_t generator_random_next(GeneratorRandom *random) {
  uint32_t x = random->state;
  if (x == 0)
    x = 0x6d2b79f5u;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  random->state = x;
  return x;
}

int generator_random_int(GeneratorRandom *random, int limit) {
  if (limit <= 1)
    return 0;
  return (int)(generator_random_next(random) % (uint32_t)limit);
}

void shuffle_ints(GeneratorRandom *random, int *values, int count) {
  for (int i = count - 1; i > 0; i--) {
    int j = generator_random_int(random, i + 1);
    int tmp = values[i];
    values[i] = values[j];
    values[j] = tmp;
  }
}