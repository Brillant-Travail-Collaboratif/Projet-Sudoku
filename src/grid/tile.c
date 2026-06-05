#include "tile_internal.h"

Affectation history[81];
int history_index;

SudokuTile *create_tile(const char value,
                        const char possible[NUMBER_OF_POSSIBLE]) {
  if (is_value_valid(value) || is_possibles_valid(possible))
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

void delete_tile(SudokuTile *tile) {
  if (tile != NULL)
    free(tile);
}

char is_value_valid(char value) {
  if (value < 0 || value > NUMBER_OF_POSSIBLE) {
    return 1;
  }
  return 0;
}

char is_possibles_valid(const char possible[NUMBER_OF_POSSIBLE]) {
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    if (is_value_valid(possible[i]))
      return 1;
  }
  return 0;
}

char get_tile_value(SudokuTile *tile) {
  if (tile == NULL || is_value_valid(tile->value))
    return 0;

  return tile->value;
}

char *get_tile_possibles(SudokuTile *tile) {
  if (tile == NULL || is_possibles_valid(tile->possible))
    return NULL;

  return tile->possible;
}

char set_tile_value(SudokuTile *tile, char value, unsigned char supposed) {
  if (tile == NULL || is_value_valid(value))
    return 1;

  tile->value = value;

  if (value != 0) {
    for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++)
      tile->possible[d] = 0;
    tile->possible[value - 1] = 1;
  }

  if (value != 0) {
    if (history_index > 80)
      return 1;
    history[history_index].tile = tile;
    history[history_index].value = value;
    history[history_index++].supposed = supposed;
  }
  return 0;
}

char set_tile_possibles(SudokuTile *tile, char possible[NUMBER_OF_POSSIBLE]) {
  if (tile == NULL || is_possibles_valid(possible))
    return 1;

  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    tile->possible[i] = possible[i];
  }
  return 0;
}