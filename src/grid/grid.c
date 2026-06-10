#include "grid_internal.h"

Grid create_grid() {
  Grid grid = malloc(sizeof(GridData));
  if (grid == NULL)
    return NULL;

  grid->allTiles = malloc(sizeof(SudokuTile) * NUMBER_OF_TILE_IN_A_GRID);
  if (grid->allTiles == NULL) {
    free(grid);
    return NULL;
  }

  for (int i = 0; i < SUBSET_COUNT; i++)
    grid->allSubsets.subsets[i] = NULL;

  char possible[NUMBER_OF_POSSIBLE];
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    possible[i] = 1;
  }
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    grid->allTiles[i].value = 0;
    for (unsigned char j = 0; j < NUMBER_OF_POSSIBLE; j++)
      grid->allTiles[i].possible[j] = possible[j];
  }

  if (build_all_subsets(grid) != 0) {
    free(grid->allTiles);
    free(grid);
    return NULL;
  }

  return grid;
}

void delete_grid(Grid grid) {
  if (grid == NULL)
    return;
  free_all_subsets(&grid->allSubsets);
  free(grid->allTiles);
  free(grid);
}

char get_grid_value_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || grid->allTiles == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return 0;

  return get_tile_value(&grid->allTiles[(y - 1) * 9 + (x - 1)]);
}

char *get_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || grid->allTiles == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return NULL;

  return get_tile_possibles(&grid->allTiles[(y - 1) * 9 + (x - 1)]);
}

char set_grid_value_xy(Grid grid, unsigned char x, unsigned char y, char value,
                       unsigned char supposed) {
  if (grid == NULL || grid->allTiles == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9) {
    return 1;
  }

  return set_tile_value(&grid->allTiles[(y - 1) * 9 + (x - 1)], value,
                        supposed);
}

char set_grid_possibles_xy(Grid grid, unsigned char x, unsigned char y,
                           char possible[NUMBER_OF_POSSIBLE]) {
  if (grid == NULL || grid->allTiles == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return 1;

  return set_tile_possibles(&grid->allTiles[(y - 1) * 9 + (x - 1)], possible);
}

int grid_filled_count(Grid grid) {
  if (grid == NULL || grid->allTiles == NULL)
    return 0;
  int count = 0;
  for (int i = 0; i < GRID_SIZE; i++)
    if (grid->allTiles[i].value != 0)
      count++;
  return count;
}