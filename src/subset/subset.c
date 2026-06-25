#include "subset_internal.h"

Subset allocate_subset(void) {
  return malloc(sizeof(SudokuCell *) * CELLS_PER_UNIT);
}
Subset get_row_subset(Grid grid, int rowIndex) {
  if (grid == NULL || grid->cells == NULL || rowIndex < 0 ||
      rowIndex >= CELLS_PER_UNIT)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < CELLS_PER_UNIT; k++)
    subset[k] = &grid->cells[rowIndex * CELLS_PER_UNIT + k];

  return subset;
}

Subset get_column_subset(Grid grid, int columnIndex) {
  if (grid == NULL || grid->cells == NULL || columnIndex < 0 ||
      columnIndex >= CELLS_PER_UNIT)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < CELLS_PER_UNIT; k++)
    subset[k] = &grid->cells[k * CELLS_PER_UNIT + columnIndex];

  return subset;
}

void delete_subset(Subset subset) {
  if (subset != NULL)
    free(subset);
}

Subset get_box_subset(Grid grid, int boxIndex) {
  if (grid == NULL || grid->cells == NULL || boxIndex < 0 ||
      boxIndex >= CELLS_PER_UNIT)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  unsigned char boxRow = (boxIndex / BOX_SIDE) * BOX_SIDE;
  unsigned char boxColumn = (boxIndex % BOX_SIDE) * BOX_SIDE;

  /* Traverse the box by row, then column. */
  for (unsigned char i = 0; i < BOX_SIDE; i++) {
    for (unsigned char j = 0; j < BOX_SIDE; j++) {
      unsigned char row = boxRow + i;
      unsigned char column = boxColumn + j;
      subset[i * BOX_SIDE + j] = &grid->cells[row * CELLS_PER_UNIT + column];
    }
  }

  return subset;
}

void free_all_subsets(SubsetCollection *subsets) {
  if (subsets == NULL)
    return;
  for (int i = 0; i < SUBSET_COUNT; i++) {
    delete_subset(subsets->items[i]);
    subsets->items[i] = NULL;
  }
}

char build_all_subsets(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return 1;

  SubsetCollection *subsets = &grid->subsets;
  free_all_subsets(subsets);

  /* Subsets are stored as 9 rows, 9 columns, then 9 boxes. */
  for (int unitIndex = 0; unitIndex < CELLS_PER_UNIT; unitIndex++) {
    subsets->items[unitIndex] = get_row_subset(grid, unitIndex);
    subsets->items[9 + unitIndex] = get_column_subset(grid, unitIndex);
    subsets->items[18 + unitIndex] = get_box_subset(grid, unitIndex);
  }

  for (int i = 0; i < SUBSET_COUNT; i++) {
    if (subsets->items[i] == NULL) {
      free_all_subsets(subsets);
      return 1;
    }
  }
  return 0;
}
