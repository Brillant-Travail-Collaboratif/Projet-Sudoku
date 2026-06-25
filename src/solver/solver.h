#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/cell.h"
#include "../grid/grid.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudoku_cell.h"

char is_grid_valid(Grid grid);
char solve(Grid grid);
char solve_with_stats(Grid grid, int *guessCount);

#endif /* SOLVER_H */
