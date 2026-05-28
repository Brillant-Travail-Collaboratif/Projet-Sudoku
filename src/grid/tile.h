#ifndef TILE_H
#define TILE_H

#include "../sudoku_types/type_sudokutile.h"

SudokuTile *createTile(const char value,
                       const char possible[NUMBER_OF_POSSIBLE]);
void deleteTile(SudokuTile *tile);
char isValueValid(char value);
char isPossibleValid(const char possible[NUMBER_OF_POSSIBLE]);

#endif /* TILE_H */
