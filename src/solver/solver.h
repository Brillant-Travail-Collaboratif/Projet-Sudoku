#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../grid/tile.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudokutile.h"

extern Affectation history[GRID_SIZE];

char is_grid_valid(Grid grid);
char solve(Grid grid);
char solve_with_stats(Grid grid, int *supposition_count);

#endif /* SOLVER_H */