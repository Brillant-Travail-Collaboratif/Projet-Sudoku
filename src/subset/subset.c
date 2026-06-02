//
// Created by Meike Rayan on 02/06/2026.
//

#include "subset_internal.h"

Subset allocateSubset(void) {
    return malloc(sizeof(SudokuTile *) * SUBSET_SIZE);
}
Subset getLineSubset(Grid grid, int n) {
    if (grid == NULL ||  n < 0 || n >= SUBSET_SIZE) return NULL;

    Subset subset = allocateSubset();
    if (subset == NULL) return NULL;

    for (int k = 0; k < SUBSET_SIZE; k++)
        subset[k] = &grid[n * SUBSET_SIZE + k];

    return subset;
}

void deleteSubset(Subset subset) {
    if (subset != NULL) free(subset);
}