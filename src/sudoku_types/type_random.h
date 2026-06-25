#ifndef TYPE_RANDOM_H
#define TYPE_RANDOM_H

#include <stdint.h>

typedef struct RandomGenerator {
  uint32_t state;
} RandomGenerator;

#endif /* TYPE_RANDOM_H */