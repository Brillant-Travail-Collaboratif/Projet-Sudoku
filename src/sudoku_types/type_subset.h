#ifndef TYPE_SUBSET_H
#define TYPE_SUBSET_H

#define SUBSET_COUNT 27

#include "type_sudokutile.h"

typedef SudokuTile **Subset;

typedef struct {
  Subset subsets[SUBSET_COUNT];
} AllSubsets;

#endif /* TYPE_SUBSET_H */
