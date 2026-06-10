#ifndef SUBSET_H
#define SUBSET_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_subset.h"

/*
 * get_line_subset : sous-ensemble correspondant a la ligne n (0..8).
 */
Subset get_line_subset(Grid grid, int n);

/*
 * get_col_subset : sous-ensemble correspondant a la colonne n (0..8).
 */
Subset get_col_subset(Grid grid, int n);

/*
 * get_subsq_subset : sous-ensemble correspondant au sous-carre n (0..8).
 * Numerotation : de gauche a droite puis de haut en bas (0 = haut-gauche,
 * 4 = centre, 8 = bas-droite).
 */
Subset get_subsq_subset(Grid grid, int n);

/*
 * delete_subset : libere le tableau de pointeurs alloue par getXxxSubset.
 */
void delete_subset(Subset subset);

char build_all_subsets(Grid grid);

void free_all_subsets(AllSubsets *all);

#endif /* SUBSET_H */
