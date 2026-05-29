#include "tile_internal.h"

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
  if (value < 0 || value > NUMBER_OF_POSSIBLE) {
    return 1;
  }
  return 0;
}

char isPossibleValid(const char possible[NUMBER_OF_POSSIBLE]) {
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    if (isValueValid(possible[i]))
      return 1;
  }
  return 0;
}

char tileGetValue(SudokuTile *tile) {
  if (tile == NULL || isValueValid(tile->value))
    return 0;

  return tile->value;
}

char *tileGetPossible(SudokuTile *tile) {
  if (tile == NULL || !isPossibleValid(tile->possible))
    return NULL;

  return tile->possible;
}

char tileSetValue(SudokuTile *tile, char value) {
  if (tile == NULL || isValueValid(value)) {
    return 1;
  }

  tile->value = value;
  return 0;
}

char tileSetPossible(SudokuTile *tile, char possible[NUMBER_OF_POSSIBLE]) {
  if (tile == NULL || !isPossibleValid(possible))
    return 1;

  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    tile->possible[i] = possible[i];
  }
  return 0;
}