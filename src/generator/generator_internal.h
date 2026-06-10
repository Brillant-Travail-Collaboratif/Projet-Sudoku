#ifndef GENERATOR_INTERNAL_H
#define GENERATOR_INTERNAL_H

#include "../grid/grid.h"
#include "../sudoku_types/type_difficulty.h"
#include "../solver/solver.h"
#include "generator.h"
#include "random.h"
#include "difficulty.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_GENERATION_ATTEMPTS 250
#define MAX_REMOVED_FOR_GUESSED_LEVELS 70

char is_value_allowed(Grid grid, int index, char value);
int collect_allowed_values(Grid grid, int index, char values[GRID_SIDE]);
int find_best_empty_cell(Grid grid);
char fill_complete_grid(Grid grid, GeneratorRandom *random);
char grids_have_same_values(Grid a, Grid b);
char solution_matches_completed_grid(Grid puzzle, Grid completed);
char count_solutions(Grid grid, int index, int *count, Grid first_solution);
char has_unique_matching_solution(Grid puzzle, Grid completed);
int target_removed_count(Difficulty difficulty, GeneratorRandom *random);
Grid build_candidate_puzzle(Grid completed, Difficulty difficulty,
                            GeneratorRandom *random);

#endif /* GENERATOR_INTERNAL_H */