#include "tile.h"
#include "grid_internal.h"

SudokuTile *createTile(const char value,
                       const char possible[NUMBER_OF_POSSIBLE]) {
  if (isValueValid(value) || isPossibleValid(possible))
    return NULL;

  SudokuTile *tile = malloc(sizeof(SudokuTile));
  if (tile == NULL)
    return NULL;

  tile->value = value;
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    tile->possible[i] = possible[i];
  }

  return tile;
}

void deleteTile(SudokuTile *tile) {
  if (tile != NULL)
    free(tile);
}

char isValueValid(char value) {
  if (value < 0 || value > NUMBER_OF_POSSIBLE)
    return 1;
  return 0;
}

char isPossibleValid(const char possible[NUMBER_OF_POSSIBLE]) {
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    if (isValueValid(possible[i]))
      return 1;
  }
  return 0;
}