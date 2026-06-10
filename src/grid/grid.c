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

Grid clone_grid_values(Grid grid) {
  if (grid == NULL || grid->allTiles == NULL)
    return NULL;

  Grid clone = create_grid();
  if (clone == NULL)
    return NULL;

  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    clone->allTiles[i].value = grid->allTiles[i].value;
    for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++)
      clone->allTiles[i].possible[d] = grid->allTiles[i].possible[d];
  }

  return clone;
}

void reset_grid_candidates(Grid grid) {
  if (grid == NULL || grid->allTiles == NULL)
    return;

  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++)
      grid->allTiles[i].possible[d] = grid->allTiles[i].value == 0 ? 1 : 0;
    if (grid->allTiles[i].value >= 1 &&
        grid->allTiles[i].value <= NUMBER_OF_POSSIBLE)
      grid->allTiles[i].possible[grid->allTiles[i].value - 1] = 1;
  }
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

char set_grid_value_raw(Grid grid, unsigned char x, unsigned char y,
                        char value) {
  if (grid == NULL || grid->allTiles == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9 || value < 0 || value > NUMBER_OF_POSSIBLE)
    return 1;

  SudokuTile *tile = &grid->allTiles[(y - 1) * 9 + (x - 1)];
  tile->value = value;
  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++)
    tile->possible[d] = value == 0 ? 1 : 0;
  if (value != 0)
    tile->possible[value - 1] = 1;

  return 0;
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