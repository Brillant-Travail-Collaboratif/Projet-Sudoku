#ifndef SUBSET_H
#define SUBSET_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_subset.h"

#define SUBSET_COUNT 27

typedef struct {
    Subset subsets[SUBSET_COUNT];
} AllSubsets;

/*
 * getLineSubset : sous-ensemble correspondant a la ligne n (0..8).
 */
Subset getLineSubset(Grid grid, int n);

/*
 * getColSubset : sous-ensemble correspondant a la colonne n (0..8).
 */
Subset getColSubset(Grid grid, int n);

/*
 * getSubsqSubset : sous-ensemble correspondant au sous-carre n (0..8).
 * Numerotation : de gauche a droite puis de haut en bas (0 = haut-gauche,
 * 4 = centre, 8 = bas-droite).
 */
Subset getSubsqSubset(Grid grid, int n);

/*
 * deleteSubset : libere le tableau de pointeurs alloue par getXxxSubset.
 */
void deleteSubset(Subset subset);

char buildAllSubsets(Grid grid, AllSubsets *all);

void freeAllSubsets(AllSubsets *all);

#endif /* SUBSET_H */
