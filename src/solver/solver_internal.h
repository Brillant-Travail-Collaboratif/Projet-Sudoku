#ifndef SOLVER_INTERNAL_H
#define SOLVER_INTERNAL_H

#include <stddef.h>

#include "solver.h"
#include "../sudoku_types/type_sudokutile.h"

unsigned char countCandidates(const SudokuTile *tile, char *candidate);

void fixTileValue(SudokuTile *tile, char value);

#endif /* SOLVER_INTERNAL_H */
