#ifndef GRID_H
#define GRID_H

#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_sudoku_cell.h"

Grid create_grid(void);
void delete_grid(Grid grid);
Grid clone_grid_values(Grid grid);
void reset_grid_candidates(Grid grid);
char get_grid_value_xy(Grid grid, unsigned char x, unsigned char y);
char *get_grid_candidates_xy(Grid grid, unsigned char x, unsigned char y);
char set_grid_value_xy(Grid grid, unsigned char x, unsigned char y, char value,
                       unsigned char isGuess);
char set_grid_value_raw(Grid grid, unsigned char x, unsigned char y,
                        char value);
char set_grid_candidates_xy(Grid grid, unsigned char x, unsigned char y,
                           char candidates[CANDIDATE_COUNT]);

int count_filled_cells(Grid grid);
#endif /* GRID_H */
