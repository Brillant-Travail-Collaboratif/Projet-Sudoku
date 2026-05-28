#ifndef GRID_H
#define GRID_H

#include "../sudoku_types/type_grid.h"

Grid createGrid();
void deleteGrid(Grid grid);
SudokuTile *gridGetTileXY(Grid grid, unsigned char x, unsigned char y);
char gridGetValueXY(Grid grid, unsigned char x, unsigned char y);
char *gridGetPossibleXY(Grid grid, unsigned char x, unsigned char y);
char gridSetTileXY(Grid grid, unsigned char x, unsigned char y,
                   SudokuTile *tile);
char gridSetValueXY(Grid grid, unsigned char x, unsigned char y, char value);
char gridSetPossibleXY(Grid grid, unsigned char x, unsigned char y,
                       char possible[NUMBER_OF_POSSIBLE]);

#endif /* GRID_H */
