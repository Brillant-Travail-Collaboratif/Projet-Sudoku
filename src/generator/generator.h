#ifndef GENERATOR_H
#define GENERATOR_H

#include "../grid/grid.h"
#include "../sudoku_types/type_difficulty.h"

Grid generate_sudoku(Difficulty difficulty, unsigned int seed);
char parse_difficulty(const char *text, Difficulty *difficulty);
const char *difficulty_to_string(Difficulty difficulty);

#endif /* GENERATOR_H */