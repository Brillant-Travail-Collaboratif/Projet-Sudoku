#ifndef TYPE_HISTORY_H
#define TYPE_HISTORY_H

#include "type_grid.h"

typedef struct SolverStep {
  SudokuCell *cell;
  char isGuess;
  char value;
  char gridValues[GRID_CELL_COUNT];
  char gridCandidates[GRID_CELL_COUNT][GRID_SIDE];
} SolverStep;

extern SolverStep solverHistory[GRID_CELL_COUNT];
extern int historyIndex;
extern int deductionCount;

#endif /* TYPE_HISTORY_H */
