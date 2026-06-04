#ifndef GRID_H
#define GRID_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_sudokutile.h"

extern SudokuTile grid[GRID_SIZE];

Grid createGrid();
void deleteGrid(Grid grid);
char gridGetValueXY(Grid grid, unsigned char x, unsigned char y);
char *gridGetPossibleXY(Grid grid, unsigned char x, unsigned char y);
char gridSetValueXY(Grid grid, unsigned char x, unsigned char y, char value,
                    unsigned char supposed);
char gridSetPossibleXY(Grid grid, unsigned char x, unsigned char y,
                       char possible[NUMBER_OF_POSSIBLE]);

#endif /* GRID_H */
