#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../grid/tile.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudokutile.h"

char solve_hidden_singles_in_line(SudokuTile *line);
char solveNakedSingles(Grid grid);
char cleanLine(SudokuTile *line);
char cleanSubset(Subset s);
char clean_grid(Grid grid);
char solve_hidden_singles(Grid grid);

#endif /* SOLVER_H */
