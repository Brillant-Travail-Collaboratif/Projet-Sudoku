#ifndef TILE_INTERNAL_H
#define TILE_INTERNAL_H

#include "../sudoku_types/type_history.h"
#include "tile.h"
#include <stdlib.h>

char isValueValid(char value);
char isPossibleValid(const char possible[NUMBER_OF_POSSIBLE]);

#endif /* TILE_INTERNAL_H */
