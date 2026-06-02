//
// Created by Nicolas Gouaux on 02/06/2026.
//

#include "subset_internal.h"

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

Subset getColSubset(Grid grid, int n) {
  if (grid == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocateSubset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < SUBSET_SIZE; k++)
    subset[k] = &grid[k * SUBSET_SIZE + n];

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

void freeAllSubsets(AllSubsets *all) {
  if (all == NULL)
    return;
  for (int i = 0; i < SUBSET_COUNT; i++) {
    deleteSubset(all->subsets[i]);
    all->subsets[i] = NULL;
  }
}

char buildAllSubsets(Grid grid, AllSubsets *all) {
  if (grid == NULL || all == NULL)
    return 1;

  for (int i = 0; i < SUBSET_COUNT; i++)
    all->subsets[i] = NULL;

  for (int n = 0; n < SUBSET_SIZE; n++) {
    all->subsets[n] = getLineSubset(grid, n);       /*  0..8  : lignes      */
    all->subsets[9 + n] = getColSubset(grid, n);    /*  9..17 : colonnes    */
    all->subsets[18 + n] = getSubsqSubset(grid, n); /* 18..26 : sous-carres */
  }

  for (int i = 0; i < SUBSET_COUNT; i++) {
    if (all->subsets[i] == NULL) {
      freeAllSubsets(all);
      return 1;
    }
  }
  return 0;
}