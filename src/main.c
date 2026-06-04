#include "io/display.h"
#include "io/read.h"
#include "solver/solver.h"
#include "sudoku_types/type_grid.h"
#include <ncurses.h>

int main(int argc, char **argv) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  Grid grid = loadSudokuFromFile("../doc/intermediate_table_1.txt");

  req_start_grid(grid);

  solve(grid);

  displayFinal(grid);

  getch();
  endwin();
  if (argc && argv)
    return 0;
  return 0;
}
