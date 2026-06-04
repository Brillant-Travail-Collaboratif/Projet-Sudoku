#include "grid_internal.h"

Grid createGrid() {
  Grid grid = malloc(sizeof(SudokuTile) * NUMBER_OF_TILE_IN_A_GRID);
  if (grid == NULL)
    return NULL;

  char possible[NUMBER_OF_POSSIBLE];
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    possible[i] = 1;
  }
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    SudokuTile *newTile = createTile(0, possible);
    grid[i] = *newTile;
  }

  return grid;
}

void deleteGrid(Grid grid) {
  if (grid == NULL)
    return;
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    deleteTile(&grid[i]);
  }
  free(grid);
}

char gridGetValueXY(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2)
    return 0;

  return tileGetValue(&grid[(y - 1) * 9 + (x - 1)]);
}

char *gridGetPossibleXY(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2)
    return NULL;

  return tileGetPossible(&grid[(y - 1) * 9 + (x - 1)]);
}

char gridSetValueXY(Grid grid, unsigned char x, unsigned char y, char value,
                    unsigned char supposed) {
  if (grid == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2) {
    return 1;
  }

  return tileSetValue(&grid[(y - 1) * 9 + (x - 1)], value, supposed);
}
char gridSetPossibleXY(Grid grid, unsigned char x, unsigned char y,
                       char possible[NUMBER_OF_POSSIBLE]) {
  if (grid == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2)
    return 1;

  return tileSetPossible(&grid[(y - 1) * 9 + (x - 1)], possible);
}

int grid_filled_count(Grid grid) {
  int count = 0;
  for (int i = 0; i < GRID_SIZE; i++)
    if (grid[i].value != 0)
      count++;
  return count;
}