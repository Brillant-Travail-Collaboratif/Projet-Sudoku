#ifndef TILE_H
#define TILE_H

#include "../sudoku_types/type_sudokutile.h"

SudokuTile *createTile(const char value,
                       const char possible[NUMBER_OF_POSSIBLE]);
void deleteTile(SudokuTile *tile);

char tileGetValue(SudokuTile *tile);
char *tileGetPossible(SudokuTile *tile);
char tileSetValue(SudokuTile *tile, char value, unsigned char supposed);
char tileSetPossible(SudokuTile *tile, char possible[NUMBER_OF_POSSIBLE]);

#endif /* TILE_H */
