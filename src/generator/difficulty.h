#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "../sudoku_types/type_difficulty.h"
#include "../sudoku_types/type_grid.h"

DifficultyTarget target_for_difficulty(Difficulty difficulty);
const char *difficulty_to_string(Difficulty difficulty);
char parse_difficulty(const char *text, Difficulty *difficulty);
char matches_difficulty(Grid puzzle, Difficulty difficulty);
int removed_count(Grid grid);

#endif /* DIFFICULTY_H */