#ifndef SOLVER_INTERNAL_H
#define SOLVER_INTERNAL_H

#define TILES_PER_LINE 9
#define MAX_SOLVE_ROUNDS 5000L

#include "../subset/subset.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudokutile.h"
#include "solver.h"
#include <stddef.h>
#include <stdlib.h>

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
char guess_value(Grid grid);

void back_play(Grid grid);
void solver_reset_history(void);
int solver_count_history_suppositions(void);

unsigned char count_candidates(const SudokuTile *tile, char *candidate);
char remove_tile_possible(SudokuTile *tile, char value);
char solveHiddenSinglesInSubset(Subset s);
char clean_hidden_pairs_in_subset(Subset subset);

char apply_rule_on_grid(Grid grid, char (*rule)(Subset));
unsigned char list_candidates(SudokuTile *t, char out[NUMBER_OF_POSSIBLE]);

void deduce_until_stable(Grid grid);
char has_pending_supposition();

#endif /* SOLVER_INTERNAL_H */
