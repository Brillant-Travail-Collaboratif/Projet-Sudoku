#include "grid.h"
#include "grid_internal.h"
#include "tile.h"

Grid createGrid() {
  Grid grid = malloc(sizeof(SudokuTile) * NUMBER_OF_TILE_IN_A_GRID);
  if (grid == NULL)
    return NULL;

  char possible[NUMBER_OF_POSSIBLE] = {0};
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

SudokuTile *gridGetTileXY(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2)
    return NULL;

  return &grid[(y - 1) * 9 + (x - 1)];
}

char gridGetValueXY(Grid grid, unsigned char x, unsigned char y) {
  SudokuTile *tile = gridGetTileXY(grid, x, y);
  if (tile == NULL)
    return NULL;
  return tile->value;
}

char *gridGetPossibleXY(Grid grid, unsigned char x, unsigned char y) {
  SudokuTile *tile = gridGetTileXY(grid, x, y);
  if (tile == NULL)
    return NULL;
  return tile->possible;
}

char gridSetTileXY(Grid grid, unsigned char x, unsigned char y,
                   SudokuTile *tile) {
  if (grid == NULL || tile == NULL || x > NUMBER_OF_TILE_IN_A_GRID / 2 ||
      y > NUMBER_OF_TILE_IN_A_GRID / 2)
    return NULL;

  grid[(y - 1) * 9 + (x - 1)] = *tile;
}

char gridSetValueXY(Grid grid, unsigned char x, unsigned char y, char value);
char gridSetPossibleXY(Grid grid, unsigned char x, unsigned char y,
                       char possible[NUMBER_OF_POSSIBLE]);