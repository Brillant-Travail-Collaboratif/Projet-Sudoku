#include "display_internal.h"

void displayFinal(Grid grid) {
  if (grid == NULL) return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0) printf("%s", ROW_SEPARATOR_FINAL);
    for (unsigned char x = 1; x <= GRID_SIDE; x++) {
      if ((x - 1) % BOX_SIDE == 0) printf("| ");
      const char value = gridGetValueXY(grid, x, y);
      printf("%c ", value == 0 ? ' ' : (char)('0' + value));
    }
    printf("|\n");
  }
  printf("%s", ROW_SEPARATOR_FINAL);
}

void displayPossible(Grid grid) {
  if (grid == NULL) return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0) printf("%s", ROW_SEPARATOR_POSSIBLE);
    for (unsigned char band = 0; band < BOX_SIDE; band++) {
      for (unsigned char x = 1; x <= GRID_SIDE; x++) {
        if ((x - 1) % BOX_SIDE == 0) printf("| ");
        const char value = gridGetValueXY(grid, x, y);
        const char *possible = gridGetPossibleXY(grid, x, y);
        for (unsigned char k = 0; k < BOX_SIDE; k++) {
          const char digit = (char)(band * BOX_SIDE + k + 1);
          if (value != 0) {
            printf("%c", value == digit ? (char)('0' + digit) : '*');
          } else {
            const char isCandidate = possible != NULL && possible[digit - 1];
            printf("%c", isCandidate ? (char)('0' + digit) : ' ');
          }
        }
        printf(" ");
      }
      printf("|\n");
    }
  }
  printf("%s", ROW_SEPARATOR_POSSIBLE);
}
