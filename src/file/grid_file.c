#include "grid_file.h"

Grid load_grid_from_file(const char *fileName) {
  if (fileName == NULL)
    return NULL;

  FILE *file = fopen(fileName, "r");
  if (file == NULL)
    return NULL;

  Grid grid = create_grid();
  if (grid == NULL) {
    fclose(file);
    return NULL;
  }

  for (unsigned char row = 0; row < 9; row++) {
    for (unsigned char column = 0; column < 9; column++) {
      char symbol;
      if (fscanf(file, " %c", &symbol) != 1 ||
          (symbol != '?' && (symbol < '1' || symbol > '9'))) {
        delete_grid(grid);
        fclose(file);
        return NULL;
      }

      char value = symbol == '?' ? 0 : (char)(symbol - '0');
      if (set_grid_value_raw(grid, column + 1, row + 1, value) != 0) {
        delete_grid(grid);
        fclose(file);
        return NULL;
      }
    }
  }

  char extra;
  if (fscanf(file, " %c", &extra) == 1) {
    delete_grid(grid);
    fclose(file);
    return NULL;
  }

  fclose(file);
  return grid;
}

int write_grid_to_file(const char *fileName, Grid grid) {
  if ((fileName == NULL) || (grid == NULL))
    return 1;
  FILE *file = fopen(fileName, "w");
  if (file == NULL)
    return 1;
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
