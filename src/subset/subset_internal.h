#ifndef SUBSET_INTERNAL_H
#define SUBSET_INTERNAL_H

#include "subset.h"
#include <stddef.h>
#include <stdlib.h>

/* Dimensions d'un sous-ensemble et d'un sous-carre (3x3). */
#define SUBSET_SIZE 9
#define BOX_SIDE 3

/*
 * allocate_subset : alloue le tableau de 9 pointeurs SudokuTile*.
 * Retourne NULL en cas d'echec malloc.
 */
Subset allocate_subset(void);

#endif /* SUBSET_INTERNAL_H */
