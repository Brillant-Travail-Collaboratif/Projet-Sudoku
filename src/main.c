#include "generator/difficulty.h"
#include "generator/generator.h"
#include "grid/grid.h"
#include "io/display.h"
#include "solver/solver.h"
#include "sudoku_types/type_grid.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  Difficulty difficulty = BASIC;
  unsigned int seed = 1;
  char verbose = 0;
  char interactive = 0;

  for (int i = 1; i < argc; i++) {
    Difficulty parsed_difficulty;
    char *end = NULL;
    unsigned long parsed_seed;

    if (strcmp(argv[i], "-v") == 0) {
      verbose = 1;
    } else if (strcmp(argv[i], "-i") == 0)
      interactive = 1;
    else if (parse_difficulty(argv[i], &parsed_difficulty)) {
      difficulty = parsed_difficulty;
    } else {
      parsed_seed = strtoul(argv[i], &end, 10);
      if (end != argv[i] && *end == '\0')
        seed = (unsigned int)parsed_seed;
    }
  }

  SCREEN *screen = newterm(NULL, stdout, stdin);
  if (screen == NULL)
    return 1;
  set_term(screen);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  printw("Generating Sudoku %d with seed %d, please wait ...", difficulty,
         seed);
  refresh();
  Grid grid = generate_sudoku(difficulty, seed);
  if (grid == NULL) {
    endwin();
    delscreen(screen);
    fprintf(stderr, "Could not generate a %s sudoku with seed %u\n",
            difficulty_to_string(difficulty), seed);
    return 1;
  }

  display_values(grid);
  printw("Generated %s sudoku with seed %u\n", difficulty_to_string(difficulty),
         seed);
  printw("Press any key to solve\n");
  refresh();
  if (interactive)
    getch();

  int suppositions = 0;
  solve_with_stats(grid, &suppositions);

  display_values(grid);
  if (verbose) {
    printw("Suppositions used: %d\n", suppositions);
    refresh();
  }

  delete_grid(grid);
  refresh();
  if (interactive)
    getch();
  endwin();
  delscreen(screen);
  if (argc && argv)
    return 0;
  return 0;
}