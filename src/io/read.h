#ifndef PROJET_SUDOKU_READ_H
#define PROJET_SUDOKU_READ_H

#include <stdio.h>
#include "../grid/grid.h"

Grid loadSudokuFromFile(const char *filename);
int saveSudokuToFile(const char *filename, Grid grid);

#endif //PROJET_SUDOKU_READ_H
