#ifndef GENERATOR_H
#define GENERATOR_H

#include "../grid/grid.h"
#include "../sudoku_types/type_difficulty.h"

Grid generate_sudoku(Difficulty difficulty, unsigned int seed);

#endif /* GENERATOR_H */