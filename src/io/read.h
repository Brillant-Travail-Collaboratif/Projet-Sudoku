#ifndef PROJET_SUDOKU_READ_H
#define PROJET_SUDOKU_READ_H

#include "../grid/grid.h"
#include <stdio.h>

Grid load_sudoku_from_file(const char *filename);
int save_sodoku_to_file(const char *filename, Grid grid);

#endif // PROJET_SUDOKU_READ_H
