#include "display_internal.h"

void display_values(Grid grid) {
  if (grid == NULL)
    return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0)
      printf("%s", ROW_SEPARATOR_FINAL);
    for (unsigned char x = 1; x <= GRID_SIDE; x++) {
      if ((x - 1) % BOX_SIDE == 0)
        printf("| ");
      const char value = get_grid_value_xy(grid, x, y);
      printf("%c ", value == 0 ? ' ' : (char)('0' + value));
    }
    printf("|\n");
  }
  printf("%s", ROW_SEPARATOR_FINAL);
}

void display_values_curses(Grid grid) {
  clear();
  if (grid == NULL)
    return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0)
      printw("%s", ROW_SEPARATOR_FINAL);
    for (unsigned char x = 1; x <= GRID_SIDE; x++) {
      if ((x - 1) % BOX_SIDE == 0)
        printw("| ");
      const char value = get_grid_value_xy(grid, x, y);
      printw("%c ", value == 0 ? ' ' : (char)('0' + value));
    }
    printw("|\n");
  }
  printw("%s", ROW_SEPARATOR_FINAL);
  refresh();
}

void display_candidates(Grid grid) {
  clear();
  if (grid == NULL)
    return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0)
      printw("%s", ROW_SEPARATOR_CANDIDATES);
    for (unsigned char band = 0; band < BOX_SIDE; band++) {
      for (unsigned char x = 1; x <= GRID_SIDE; x++) {
        if ((x - 1) % BOX_SIDE == 0)
          printw("| ");
        const char value = get_grid_value_xy(grid, x, y);
        const char *candidates = get_grid_candidates_xy(grid, x, y);
        for (unsigned char k = 0; k < BOX_SIDE; k++) {
          const char digit = (char)(band * BOX_SIDE + k + 1);
          if (value != 0) {
            printw("%c", value == digit ? (char)('0' + digit) : '*');
          } else {
            const char isCandidate = candidates != NULL && candidates[digit - 1];
            printw("%c", isCandidate ? (char)('0' + digit) : ' ');
          }
        }
        printw(" ");
      }
      printw("|\n");
    }
  }
  printw("%s", ROW_SEPARATOR_CANDIDATES);
  refresh();
}

void display_subset(Subset subset) {
  if (subset == NULL) {
    printf("(subset NULL)\n");
    return;
  }

  printf("+---+---+---+---+---+---+---+---+---+\n| ");
  for (unsigned char i = 0; i < GRID_SIDE; i++) {
    const char value = subset[i]->value;
    printf("%c | ", value == 0 ? '.' : (char)('0' + value));
  }
  printf("\n+---+---+---+---+---+---+---+---+---+\n");

  for (unsigned char i = 0; i < GRID_SIDE; i++) {
    printf("  cell %u: ", i);
    if (subset[i]->value != 0) {
      printf("value %c\n", (char)('0' + subset[i]->value));
    } else {
      printf("candidates ");
      char first = 1;
      for (unsigned char d = 0; d < GRID_SIDE; d++) {
        if (subset[i]->candidates[d]) {
          printf("%s%u", first ? "" : ",", d + 1);
          first = 0;
        }
      }
      if (first)
        printf("(none)");
      printf("\n");
    }
  }
}
