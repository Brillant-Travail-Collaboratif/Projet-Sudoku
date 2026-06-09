#include "grid/grid.h"
#include "io/display.h"
#include "io/read.h"
#include "solver/solver.h"
#include "sudoku_types/type_grid.h"
#include <ncurses.h>

int main(int argc, char **argv) {
  SCREEN *screen = newterm(NULL, stdout, stdin);
  if (screen == NULL)
    return 1;
  set_term(screen);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  Grid grid = load_sudoku_from_file("../tables/expert_table_1.txt");

  start_grid_tui(grid);

  solve(grid);

  display_values(grid);

  delete_grid(grid);
  refresh();
  getch();
  endwin();
  delscreen(screen);
  if (argc && argv)
    return 0;
  return 0;
}
