#ifndef RANDOM_H
#define RANDOM_H

#include "../sudoku_types/type_random.h"

int generator_random_int(GeneratorRandom *random, int limit);
void shuffle_ints(GeneratorRandom *random, int *values, int count) ;

#endif /* RANDOM_H */