#ifndef TYPE_GRID_H
#define TYPE_GRID_H

#define GRID_SIDE 9
#define GRID_CELL_COUNT (GRID_SIDE * GRID_SIDE)

#include "type_subset.h"
#include "type_sudoku_cell.h"

typedef struct GridData {
  SudokuCell *cells;
  SubsetCollection subsets;
} GridData;

typedef GridData *Grid;

#endif /* TYPE_GRID_H */
