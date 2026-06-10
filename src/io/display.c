#include "display_internal.h"

void display_values(Grid grid) {
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

void display_possibles(Grid grid) {
  clear();
  if (grid == NULL)
    return;

  for (unsigned char y = 1; y <= GRID_SIDE; y++) {
    if ((y - 1) % BOX_SIDE == 0)
      printw("%s", ROW_SEPARATOR_POSSIBLE);
    for (unsigned char band = 0; band < BOX_SIDE; band++) {
      for (unsigned char x = 1; x <= GRID_SIDE; x++) {
        if ((x - 1) % BOX_SIDE == 0)
          printw("| ");
        const char value = get_grid_value_xy(grid, x, y);
        const char *possible = get_grid_possibles_xy(grid, x, y);
        for (unsigned char k = 0; k < BOX_SIDE; k++) {
          const char digit = (char)(band * BOX_SIDE + k + 1);
          if (value != 0) {
            printw("%c", value == digit ? (char)('0' + digit) : '*');
          } else {
            const char isCandidate = possible != NULL && possible[digit - 1];
            printw("%c", isCandidate ? (char)('0' + digit) : ' ');
          }
        }
        printw(" ");
      }
      printw("|\n");
    }
  }
  printw("%s", ROW_SEPARATOR_POSSIBLE);
  refresh();
}

void start_grid_tui(Grid grid) {
  display_values(grid);
  printw("Press q to quit\n");
  move(1, 2);

  int ch;
  unsigned char x = 1, y = 1; // 1, 1 is top left

  while ((ch = getch()) != 'q' && ch != '\n') {
    if (ch == KEY_UP) {
      if (y - 1 >= 1)
        y--;
    } else if (ch == KEY_DOWN) {
      if (y + 1 <= NUMBER_OF_POSSIBLE)
        y++;
    } else if (ch == KEY_LEFT) {
      if (x - 1 >= 1)
        x--;
    } else if (ch == KEY_RIGHT) {
      if (x + 1 <= NUMBER_OF_POSSIBLE)
        x++;
    } else if (ch == '?') {
      set_grid_value_xy(grid, x, y, 0, 0);
      refresh();
    } else {
      ch -= 48;
      if (ch > 0 && ch <= 10)
        set_grid_value_xy(grid, x, y, ch, 0);
    }

    unsigned char mvx, mvy;

    mvx = x * 2;
    if (x >= 4)
      mvx += 2;
    if (x >= 7)
      mvx += 2;

    mvy = y;
    if (y >= 4)
      mvy++;
    if (y >= 7)
      mvy++;

    display_values(grid);
    printw("Press q to quit\n");
    refresh();
    move(mvy, mvx);
  }
}
void display_subset(Subset s) {
  if (s == NULL) {
    printf("(subset NULL)\n");
    return;
  }

  printf("+---+---+---+---+---+---+---+---+---+\n| ");
  for (unsigned char i = 0; i < GRID_SIDE; i++) {
    const char value = s[i]->value;
    printf("%c | ", value == 0 ? '.' : (char)('0' + value));
  }
  printf("\n+---+---+---+---+---+---+---+---+---+\n");

  for (unsigned char i = 0; i < GRID_SIDE; i++) {
    printf("  case %u : ", i);
    if (s[i]->value != 0) {
      printf("valeur %c\n", (char)('0' + s[i]->value));
    } else {
      printf("candidats ");
      char first = 1;
      for (unsigned char d = 0; d < GRID_SIDE; d++) {
        if (s[i]->possible[d]) {
          printf("%s%u", first ? "" : ",", d + 1);
          first = 0;
        }
      }
      if (first)
        printf("(aucun)");
      printf("\n");
    }
  }
}
