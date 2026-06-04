#ifndef TYPE_HISTORY_H
#define TYPE_HISTORY_H

#include "type_sudokutile.h"

#define GRID_SIZE    81
#define GRID_SIDE    9

typedef struct Affectation {
  SudokuTile *tile;
  char supposed;
  char value;
} Affectation;

Affectation history[81];
int history_index;

#endif /* TYPE_HISTORY_H */
