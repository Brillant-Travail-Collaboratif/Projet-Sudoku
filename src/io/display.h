#ifndef DISPLAY_H
#define DISPLAY_H

#include "../grid/grid.h"
#include "../sudoku_types/type_subset.h"

void displayFinal(Grid grid);
void displayPossible(Grid grid);
void req_start_grid(Grid grid);
void dispSubset(Subset s);

#endif /* DISPLAY_H */
