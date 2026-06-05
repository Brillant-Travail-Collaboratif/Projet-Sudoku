#include "grid_internal.h"

static char isCoordinateInvalid(unsigned char x, unsigned char y) {
  return x < 1 || x > GRID_SIDE || y < 1 || y > GRID_SIDE;
}

Grid create_grid() {
  Grid grid = malloc(sizeof(SudokuTile) * NUMBER_OF_TILE_IN_A_GRID);
  if (grid == NULL)
    return NULL;

  char possible[NUMBER_OF_POSSIBLE];
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    possible[i] = 1;
  }
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    grid[i].value = 0;
    for (unsigned char j = 0; j < NUMBER_OF_POSSIBLE; j++)
      grid[i].possible[j] = possible[j];
  }

  return grid;
}

void delete_grid(Grid grid) {
  if (grid == NULL)
    return;
  free(grid);
}

char get_grid_value_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return 0;

  return get_tile_value(&grid[(y - 1) * 9 + (x - 1)]);
}

char *get_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return NULL;

  return get_tile_possibles(&grid[(y - 1) * 9 + (x - 1)]);
}

char set_grid_value_xy(Grid grid, unsigned char x, unsigned char y, char value,
                       unsigned char supposed) {
  if (grid == NULL || isCoordinateInvalid(x, y)) {
    return 1;
  }

  return set_tile_value(&grid[(y - 1) * 9 + (x - 1)], value, supposed);
}
char set_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y,
                           char possible[NUMBER_OF_POSSIBLE]) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return 1;

  return set_tile_possibles(&grid[(y - 1) * 9 + (x - 1)], possible);
}

int grid_filled_count(Grid grid) {
  if (grid == NULL)
    return 0;
  int count = 0;
  for (int i = 0; i < GRID_SIZE; i++)
    if (grid[i].value != 0)
      count++;
  return count;
}