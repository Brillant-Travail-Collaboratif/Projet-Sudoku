#ifndef DISPLAY_H
#define DISPLAY_H

#include "../grid/grid.h"
#include "../sudoku_types/type_subset.h"

void display_values(Grid grid);
void display_possibles(Grid grid);
void display_subset(Subset s);

#endif /* DISPLAY_H */
