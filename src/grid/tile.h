#ifndef TILE_H
#define TILE_H

#include "../sudoku_types/type_sudokutile.h"

SudokuTile *create_tile(const char value,
                        const char possible[NUMBER_OF_POSSIBLE]);
void delete_tile(SudokuTile *tile);

char get_tile_value(SudokuTile *tile);
char *get_tile_possibles(SudokuTile *tile);
char set_tile_value(SudokuTile *tile, char value, unsigned char supposed);
char set_tile_possibles(SudokuTile *tile, char possible[NUMBER_OF_POSSIBLE]);

#endif /* TILE_H */
