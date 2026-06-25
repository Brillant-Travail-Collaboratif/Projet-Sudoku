#ifndef GRID_FILE_H
#define GRID_FILE_H

#include "../grid/grid.h"
#include <stdio.h>

Grid load_grid_from_file(const char *fileName);
int write_grid_to_file(const char *fileName, Grid grid);

#endif /* GRID_FILE_H */
