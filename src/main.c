#include "grid/grid.h"
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

  Grid grid = loadSudokuFromFile("../doc/difficile_table_1.txt");

  req_start_grid(grid);

  unsigned char modified = 0;

  do {
    modified = clean_grid(grid);
    modified |= solveNakedSingles(grid);
    modified |= solve_hidden_singles(grid);
    modified |= solveNakedSingles(grid);
    modified |= clean_hidden_pairs(grid);
  } while (modified);

  req_start_grid(grid);

  getch();
  endwin();
  if (argc && argv)
    return 0;
  return 0;
}
