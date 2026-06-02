//
// Created by Meike Rayan on 02/06/2026.
//

#include "subset_internal.h"
#include <stdlib.h>

Subset allocateSubset(void) {
  return malloc(sizeof(SudokuTile *) * SUBSET_SIZE);
}
Subset getLineSubset(Grid grid, int n) {
  if (grid == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocateSubset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < SUBSET_SIZE; k++)
    subset[k] = &grid[n * SUBSET_SIZE + k];

  return subset;
}

void deleteSubset(Subset subset) {
  if (subset != NULL)
    free(subset);
}

Subset getSubsqSubset(Grid grid, int n) {
  if (grid == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocateSubset();
  if (subset == NULL)
    return NULL;

  unsigned char boxCol = (n % BOX_SIDE) * BOX_SIDE;

  for (int i = 0; i < BOX_SIDE; i++) {
    for (int j = 0; j < BOX_SIDE; j++) {
      unsigned char row = n + i;
      unsigned char col = boxCol + j;
      subset[i * BOX_SIDE + j] = &grid[row * SUBSET_SIZE + col];
    }
  }

  return subset;
}