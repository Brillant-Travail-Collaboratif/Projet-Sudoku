#ifndef SOLVER_H
#define SOLVER_H

#include "../grid/grid.h"
#include "../grid/tile.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudokutile.h"

extern Affectation history[GRID_SIZE];

char solve_hidden_singles_in_line(SudokuTile *line);
char solve_naked_singles(Grid grid);
char clean_line(SudokuTile *line);
char clean_subset(Subset s);
char clean_grid(Grid grid);
char solve_hidden_singles(Grid grid);
char clean_hidden_pairs(Grid grid);
char clean_naked_triples(Grid grid);
char clean_hidden_triples(Grid grid);

char clean_naked_pair_in_subset(Subset s);
char clean_naked_pairs(Grid grid);

char clean_naked_triple_in_subset(Subset s);
char clean_naked_triples(Grid grid);

char clean_hidden_triples_in_subset(Subset subset);
char clean_hidden_triples(Grid grid);

char is_grid_valid(Grid grid);
char guess_value(Grid grid);

void back_play(Grid grid);

char solve(Grid grid);

#endif /* SOLVER_H */