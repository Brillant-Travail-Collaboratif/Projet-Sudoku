#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../grid/tile.h"
#include "../sudoku_types/type_sudokutile.h"
#include "../sudoku_types/type_subset.h"

char solve_hidden_singles_in_line(SudokuTile *line);
char solveNakedSingles(Grid grid);
char cleanLine(SudokuTile *line);
char cleanSubset(Subset s);
#endif /* SOLVER_H */
