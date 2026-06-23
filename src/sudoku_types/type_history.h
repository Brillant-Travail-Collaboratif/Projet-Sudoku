#ifndef TYPE_HISTORY_H
#define TYPE_HISTORY_H

#include "type_sudokutile.h"

#define GRID_SIZE 81
#define GRID_SIDE 9

typedef struct Affectation {
  SudokuTile *tile;
  char supposed;
  char value;
  char grid_values[GRID_SIZE];
  char grid_possibles[GRID_SIZE][GRID_SIDE];
} Affectation;

extern Affectation history[81];
extern int history_index;
extern int deduction_count;

#endif /* TYPE_HISTORY_H */
