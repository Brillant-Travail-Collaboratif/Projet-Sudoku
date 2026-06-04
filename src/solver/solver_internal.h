#ifndef SOLVER_INTERNAL_H
#define SOLVER_INTERNAL_H

#define TILES_PER_LINE 9

#include "../subset/subset.h"
#include "../sudoku_types/type_history.h"
#include "../sudoku_types/type_subset.h"
#include "../sudoku_types/type_sudokutile.h"
#include "solver.h"
#include <stddef.h>
#include <stdlib.h>

unsigned char countCandidates(const SudokuTile *tile, char *candidate);
void fixTileValue(SudokuTile *tile, char value);
char removeCandidate(SudokuTile *tile, char value);
char solveHiddenSinglesInSubset(Subset s);
char clean_hidden_pairs_in_subset(Subset subset);

char apply_rule_on_grid(Grid grid, char (*rule)(Subset));
unsigned char list_candidates(SudokuTile *t, char out[NUMBER_OF_POSSIBLE]);

void deduce_until_stable(Grid grid);
char has_pending_supposition();

#endif /* SOLVER_INTERNAL_H */
