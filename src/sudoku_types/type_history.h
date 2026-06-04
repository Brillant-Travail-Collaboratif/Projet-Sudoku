#ifndef TYPE_HISTORY_H
#define TYPE_HISTORY_H

#include "type_sudokutile.h"

typedef struct Affectation {
  SudokuTile *tile;
  char supposed;
  char value;
} Affectation;

extern Affectation history[81];
extern int history_index;

#endif /* TYPE_HISTORY_H */
