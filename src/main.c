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

  Grid grid = loadSudokuFromFile("../doc/trivial_table_1.txt");

  displayFinal(grid);

  clean_grid(grid);
  solveNakedSingles(grid);
  solve_hidden_singles(grid);
  solveNakedSingles(grid);

  displayFinal(grid);

  getch();
  endwin();
  if (argc && argv)
    return 0;
  return 0;
}
