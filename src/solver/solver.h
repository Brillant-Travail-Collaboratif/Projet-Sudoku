#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../grid/tile.h"
#include "../sudoku_types/type_sudokutile.h"

char solve_hidden_singles_in_line(SudokuTile *line);

#endif /* SOLVER_H */
