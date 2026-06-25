#ifndef GENERATOR_INTERNAL_H
#define GENERATOR_INTERNAL_H

#include "../grid/grid.h"
#include "../solver/solver.h"
#include "../sudoku_types/type_difficulty.h"
#include "difficulty.h"
#include "generator.h"
#include "random.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_GENERATION_ATTEMPTS 250

char is_value_allowed(Grid grid, int index, char value);
int collect_allowed_values(Grid grid, int index, char values[GRID_SIDE]);
int find_best_empty_cell(Grid grid);
char fill_complete_grid(Grid grid, RandomGenerator *random);
char have_same_grid_values(Grid firstGrid, Grid secondGrid);
char does_solution_match_completed_grid(Grid puzzle, Grid completed);
char count_solutions(Grid grid, int index, int *count, Grid firstSolution);
char has_unique_matching_solution(Grid puzzle, Grid completed);
int choose_removed_cell_target(Difficulty difficulty, RandomGenerator *random);
Grid build_candidate_puzzle(Grid completed, Difficulty difficulty,
                            RandomGenerator *random);

#endif /* GENERATOR_INTERNAL_H */
