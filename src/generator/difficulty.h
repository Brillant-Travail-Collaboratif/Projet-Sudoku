#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "../sudoku_types/type_difficulty.h"
#include "../sudoku_types/type_grid.h"

DifficultyTarget get_difficulty_target(Difficulty difficulty);
const char *get_difficulty_name(Difficulty difficulty);
char parse_difficulty(const char *text, Difficulty *difficulty);
char matches_difficulty(Grid puzzle, Difficulty difficulty);
int count_removed_cells(Grid grid);

#endif /* DIFFICULTY_H */