#ifndef PROJET_SUDOKU_READ_H
#define PROJET_SUDOKU_READ_H

#include "../grid/grid.h"
#include <stdio.h>

Grid load_grid_from_file(const char *filename);
int save_grid_to_file(const char *filename, Grid grid);

#endif // PROJET_SUDOKU_READ_H
