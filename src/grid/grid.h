#ifndef GRID_H
#define GRID_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_sudokutile.h"

Grid create_grid();
void delete_grid(Grid grid);
char get_grid_value_xy(Grid grid, unsigned char x, unsigned char y);
char *get_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y);
char set_grid_value_xy(Grid grid, unsigned char x, unsigned char y, char value,
                       unsigned char supposed);
char set_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y,
                           char possible[NUMBER_OF_POSSIBLE]);

int grid_filled_count(Grid grid);
#endif /* GRID_H */
