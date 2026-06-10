//
// Created by Nicolas Gouaux on 02/06/2026.
//

#include "subset_internal.h"

Subset allocate_subset(void) {
  return malloc(sizeof(SudokuTile *) * SUBSET_SIZE);
}
Subset get_line_subset(Grid grid, int n) {
  if (grid == NULL || grid->allTiles == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < SUBSET_SIZE; k++)
    subset[k] = &grid->allTiles[n * SUBSET_SIZE + k];

  return subset;
}

Subset get_col_subset(Grid grid, int n) {
  if (grid == NULL || grid->allTiles == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  for (int k = 0; k < SUBSET_SIZE; k++)
    subset[k] = &grid->allTiles[k * SUBSET_SIZE + n];

  return subset;
}

void delete_subset(Subset subset) {
  if (subset != NULL)
    free(subset);
}

Subset get_subsq_subset(Grid grid, int n) {
  if (grid == NULL || grid->allTiles == NULL || n < 0 || n >= SUBSET_SIZE)
    return NULL;

  Subset subset = allocate_subset();
  if (subset == NULL)
    return NULL;

  /* Position du coin haut-gauche du sous-carre n. */
  unsigned char boxRow = (n / BOX_SIDE) * BOX_SIDE; /* 0, 3 ou 6 */
  unsigned char boxCol = (n % BOX_SIDE) * BOX_SIDE; /* 0, 3 ou 6 */

  /* Parcours en ligne puis en colonne dans le sous-carre. */
  for (unsigned char i = 0; i < BOX_SIDE; i++) {
    for (unsigned char j = 0; j < BOX_SIDE; j++) {
      unsigned char row = boxRow + i;
      unsigned char col = boxCol + j;
      subset[i * BOX_SIDE + j] = &grid->allTiles[row * SUBSET_SIZE + col];
    }
  }

  return subset;
}

void free_all_subsets(AllSubsets *all) {
  if (all == NULL)
    return;
  for (int i = 0; i < SUBSET_COUNT; i++) {
    delete_subset(all->subsets[i]);
    all->subsets[i] = NULL;
  }
}

char build_all_subsets(Grid grid) {
  if (grid == NULL || grid->allTiles == NULL)
    return 1;

  AllSubsets *all = &grid->allSubsets;
  free_all_subsets(all);

  for (int n = 0; n < SUBSET_SIZE; n++) {
    all->subsets[n] = get_line_subset(grid, n);       /*  0..8  : lignes      */
    all->subsets[9 + n] = get_col_subset(grid, n);    /*  9..17 : colonnes    */
    all->subsets[18 + n] = get_subsq_subset(grid, n); /* 18..26 : sous-carres */
  }

  for (int i = 0; i < SUBSET_COUNT; i++) {
    if (all->subsets[i] == NULL) {
      free_all_subsets(all);
      return 1;
    }
  }
  return 0;
}