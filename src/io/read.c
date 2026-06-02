#include "read.h"

Grid loadSudokuFromFile(const char *filename) {
  if (filename == NULL)
    return NULL;
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return NULL;
  Grid grid = createGrid();
  if (grid == NULL) {
    fclose(file);
    return NULL;
  }
  char buffer[256];
  for (unsigned char row = 0; row < 9; row++) {
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
      fclose(file);
      return NULL;
    }
    for (unsigned char column = 0; column < 9; column++) {
      char value = buffer[column * 2];
      if (value >= '1' && value <= '9')
        gridSetValueXY(grid, column + 1, row + 1, value - '0');
      else
        gridSetValueXY(grid, column + 1, row + 1, 0);
    }
  }
  fclose(file);
  return grid;
}

int saveSudokuToFile(const char *filename, Grid grid) {
  if ((filename == NULL) || (grid == NULL))
    return -1;
  FILE *file = fopen(filename, "w");
  if (file == NULL)
    return -1;
  for (unsigned char row = 0; row < 9; row++) {
    for (unsigned char column = 0; column < 9; column++) {
      int value = gridGetValueXY(grid, column, row);
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