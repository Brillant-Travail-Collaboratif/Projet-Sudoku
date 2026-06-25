#include "grid_internal.h"

Grid create_grid(void) {
  Grid grid = malloc(sizeof(GridData));
  if (grid == NULL)
    return NULL;

  grid->cells = malloc(sizeof(SudokuCell) * GRID_CELL_COUNT);
  if (grid->cells == NULL) {
    free(grid);
    return NULL;
  }

  for (int i = 0; i < SUBSET_COUNT; i++)
    grid->subsets.items[i] = NULL;

  char candidates[CANDIDATE_COUNT];
  for (unsigned char i = 0; i < CANDIDATE_COUNT; i++) {
    candidates[i] = 1;
  }
  for (unsigned char i = 0; i < GRID_CELL_COUNT; i++) {
    grid->cells[i].value = 0;
    for (unsigned char j = 0; j < CANDIDATE_COUNT; j++)
      grid->cells[i].candidates[j] = candidates[j];
  }

  if (build_all_subsets(grid) != 0) {
    free(grid->cells);
    free(grid);
    return NULL;
  }

  return grid;
}

void delete_grid(Grid grid) {
  if (grid == NULL)
    return;
  free_all_subsets(&grid->subsets);
  free(grid->cells);
  free(grid);
}

Grid clone_grid_values(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return NULL;

  Grid clone = create_grid();
  if (clone == NULL)
    return NULL;

  for (unsigned char i = 0; i < GRID_CELL_COUNT; i++) {
    clone->cells[i].value = grid->cells[i].value;
    for (unsigned char d = 0; d < CANDIDATE_COUNT; d++)
      clone->cells[i].candidates[d] = grid->cells[i].candidates[d];
  }

  return clone;
}

void reset_grid_candidates(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return;

  for (unsigned char i = 0; i < GRID_CELL_COUNT; i++) {
    for (unsigned char d = 0; d < CANDIDATE_COUNT; d++)
      grid->cells[i].candidates[d] = grid->cells[i].value == 0 ? 1 : 0;
    if (grid->cells[i].value >= 1 &&
        grid->cells[i].value <= CANDIDATE_COUNT)
      grid->cells[i].candidates[grid->cells[i].value - 1] = 1;
  }
}

char get_grid_value_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || grid->cells == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return 0;

  return get_cell_value(&grid->cells[(y - 1) * GRID_SIDE + (x - 1)]);
}

char *get_grid_candidates_xy(Grid grid, unsigned char x, unsigned char y) {
  if (grid == NULL || grid->cells == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return NULL;

  return get_cell_candidates(&grid->cells[(y - 1) * GRID_SIDE + (x - 1)]);
}

char set_grid_value_xy(Grid grid, unsigned char x, unsigned char y, char value,
                       unsigned char isGuess) {
  if (grid == NULL || grid->cells == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9) {
    return 1;
  }

  return set_cell_value(&grid->cells[(y - 1) * GRID_SIDE + (x - 1)], value,
                        isGuess);
}

char set_grid_value_raw(Grid grid, unsigned char x, unsigned char y,
                        char value) {
  if (grid == NULL || grid->cells == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9 || value < 0 || value > CANDIDATE_COUNT)
    return 1;

  SudokuCell *cell = &grid->cells[(y - 1) * GRID_SIDE + (x - 1)];
  cell->value = value;
  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++)
    cell->candidates[d] = value == 0 ? 1 : 0;
  if (value != 0)
    cell->candidates[value - 1] = 1;

  return 0;
}

char set_grid_candidates_xy(Grid grid, unsigned char x, unsigned char y,
                           char candidates[CANDIDATE_COUNT]) {
  if (grid == NULL || grid->cells == NULL || x < 1 || x > 9 || y < 1 ||
      y > 9)
    return 1;

  return set_cell_candidates(
      &grid->cells[(y - 1) * GRID_SIDE + (x - 1)], candidates);
}

int count_filled_cells(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return 0;
  int count = 0;
  for (int i = 0; i < GRID_CELL_COUNT; i++)
    if (grid->cells[i].value != 0)
      count++;
  return count;
}
