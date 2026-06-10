#include "read.h"

Grid load_sudoku_from_file(const char *filename) {
  if (filename == NULL)
    return NULL;
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return NULL;
  Grid grid = create_grid();
  if (grid == NULL) {
    fclose(file);
    return NULL;
  }
  char buffer[256];
  for (unsigned char row = 0; row < 9; row++) {
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
      delete_grid(grid);
      fclose(file);
      return NULL;
    }
    for (unsigned char column = 0; column < 9; column++) {
      char value = buffer[column * 2];
      if (value >= '1' && value <= '9')
        set_grid_value_xy(grid, column + 1, row + 1, value - '0', 0);
      else
        set_grid_value_xy(grid, column + 1, row + 1, 0, 0);
    }
  }
  fclose(file);
  return grid;
}

int save_sodoku_to_file(const char *filename, Grid grid) {
  if ((filename == NULL) || (grid == NULL))
    return -1;
  FILE *file = fopen(filename, "w");
  if (file == NULL)
    return -1;
  for (unsigned char row = 0; row < 9; row++) {
    for (unsigned char column = 0; column < 9; column++) {
      int value = get_grid_value_xy(grid, column + 1, row + 1);
      if (value == 0)
        fprintf(file, "?");
      else
        fprintf(file, "%d", value);
      if (column < 8)
        fprintf(file, " ");
    }
    fprintf(file, "\n");
  }
  fclose(file);
  return 0;
}