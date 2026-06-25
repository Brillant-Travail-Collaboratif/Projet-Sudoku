#ifndef RANDOM_H
#define RANDOM_H

#include "../sudoku_types/type_random.h"

int generate_random_int(RandomGenerator *random, int limit);
void shuffle_integers(RandomGenerator *random, int *values, int count);

#endif /* RANDOM_H */