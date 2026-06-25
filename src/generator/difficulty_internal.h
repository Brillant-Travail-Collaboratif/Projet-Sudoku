#ifndef DIFFICULTY_INTERNAL_H
#define DIFFICULTY_INTERNAL_H

#include "../grid/grid.h"
#include "../solver/solver.h"
#include "difficulty.h"
#include <limits.h>
#include <string.h>

#define MAX_REMOVED_FOR_GUESS_LEVELS 70

char measure_difficulty(Grid puzzle, int *guesses);

#endif /* DIFFICULTY_INTERNAL_H */