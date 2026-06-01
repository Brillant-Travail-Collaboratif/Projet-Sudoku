#ifndef SOLVER_INTERNAL_H
#define SOLVER_INTERNAL_H

#include <stddef.h>
#include "solver.h"
#include "../sudoku_types/type_sudokutile.h"
#define TILES_PER_LINE 9


char removeCandidate(SudokuTile *tile, char value);
#endif /* SOLVER_INTERNAL_H */
