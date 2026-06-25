#ifndef TYPE_SUBSET_H
#define TYPE_SUBSET_H

#define SUBSET_COUNT 27
#define CELLS_PER_UNIT 9
#define BOX_SIDE 3

#include "type_sudoku_cell.h"

typedef SudokuCell **Subset;

typedef struct {
  Subset items[SUBSET_COUNT];
} SubsetCollection;

#endif /* TYPE_SUBSET_H */
