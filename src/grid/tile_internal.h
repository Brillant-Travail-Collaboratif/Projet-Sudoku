#ifndef TILE_INTERNAL_H
#define TILE_INTERNAL_H

#include "../sudoku_types/type_history.h"
#include "tile.h"
#include <stdlib.h>

char is_value_valid(char value);
char is_possibles_valid(const char possible[NUMBER_OF_POSSIBLE]);

#endif /* TILE_INTERNAL_H */
