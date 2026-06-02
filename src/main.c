#include "io/display.h"
#include "io/read.h"
#include "sudoku_types/type_grid.h"
#include <ncurses.h>

int main(int argc, char **argv) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  Grid grid = loadSudokuFromFile("../doc/trivial_table_1.txt");

  req_start_grid(grid);

  getch();
  endwin();
  if (argc && argv)
    return 0;
  return 0;
}
