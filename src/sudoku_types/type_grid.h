#ifndef TYPE_GRID_H
#define TYPE_GRID_H

#define NUMBER_OF_TILE_IN_A_GRID 81

#include "type_subset.h"
#include "type_sudokutile.h"

typedef struct GridData {
  SudokuTile *allTiles;
  AllSubsets allSubsets;
} GridData;

typedef GridData *Grid;

#endif /* TYPE_GRID_H */
