#include "grid_internal.h"

static char isCoordinateInvalid(unsigned char x, unsigned char y) {
  return x < 1 || x > GRID_SIDE || y < 1 || y > GRID_SIDE;
}

Grid createGrid() {
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

void deleteGrid(Grid grid) {
  if (grid == NULL)
    return;
  free(grid);
}

char gridGetValueXY(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return 0;

  return tileGetValue(&grid[(y - 1) * 9 + (x - 1)]);
}

char *gridGetPossibleXY(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return NULL;

  return tileGetPossible(&grid[(y - 1) * 9 + (x - 1)]);
}

char gridSetValueXY(Grid grid, unsigned char x, unsigned char y, char value,
                    unsigned char supposed) {
  if (grid == NULL || isCoordinateInvalid(x, y)) {
    return 1;
  }

  return tileSetValue(&grid[(y - 1) * 9 + (x - 1)], value, supposed);
}
char gridSetPossibleXY(Grid grid, unsigned char x, unsigned char y,
                       char possible[NUMBER_OF_POSSIBLE]) {
  if (grid == NULL || isCoordinateInvalid(x, y))
    return 1;

  return tileSetPossible(&grid[(y - 1) * 9 + (x - 1)], possible);
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