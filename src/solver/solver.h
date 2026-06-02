#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../sudoku_types/type_sudokutile.h"

char solveNakedSingles(Grid grid);
char cleanLine(SudokuTile *line);

#endif /* SOLVER_H */
