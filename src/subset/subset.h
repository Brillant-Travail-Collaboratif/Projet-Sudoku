#ifndef SUBSET_H
#define SUBSET_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_subset.h"

Subset get_row_subset(Grid grid, int rowIndex);
Subset get_column_subset(Grid grid, int columnIndex);
Subset get_box_subset(Grid grid, int boxIndex);

void delete_subset(Subset subset);

char build_all_subsets(Grid grid);

void free_all_subsets(SubsetCollection *subsets);

#endif /* SUBSET_H */
