#ifndef SOLVER_INTERNAL_H
#define SOLVER_INTERNAL_H

#define MAX_SOLVE_ROUNDS 5000L

#include "../subset/subset.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudoku_cell.h"
#include "solver.h"
#include <stddef.h>
#include <stdlib.h>

char solve_hidden_singles_in_row(SudokuCell *row);
char solve_naked_singles(Grid grid);
char clean_row(SudokuCell *row);
char clean_subset(Subset subset);
char clean_grid(Grid grid);
char solve_hidden_singles(Grid grid);
char clean_hidden_pairs(Grid grid);
char clean_naked_triples(Grid grid);
char clean_hidden_triples(Grid grid);

char clean_naked_pair_in_subset(Subset subset);
char clean_naked_pairs(Grid grid);

char clean_naked_triple_in_subset(Subset subset);
char clean_naked_triples(Grid grid);

char clean_hidden_triples_in_subset(Subset subset);
char clean_hidden_triples(Grid grid);
char guess_cell_value(Grid grid);

void backtrack_last_guess(Grid grid);
void reset_solver_history(void);
int count_history_guesses(void);

unsigned char count_candidates(const SudokuCell *cell, char *candidate);
char remove_cell_candidate(SudokuCell *cell, char value);
char solve_hidden_singles_in_subset(Subset subset);
char clean_hidden_pairs_in_subset(Subset subset);

char apply_rule_on_grid(Grid grid, char (*rule)(Subset));
unsigned char list_candidates(SudokuCell *cell,
                              char output[CANDIDATE_COUNT]);

void deduce_until_stable(Grid grid);
char has_pending_guess(void);

#endif /* SOLVER_INTERNAL_H */
