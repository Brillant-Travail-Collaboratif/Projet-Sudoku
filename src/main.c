#include "generator/difficulty.h"
#include "generator/generator.h"
#include "grid/grid.h"
#include "io/display.h"
#include "io/read.h"
#include "solver/solver.h"
#include "sudoku_types/type_grid.h"
#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static int cmp_strings(const void *a, const void *b) {
  return strcmp(*(const char *const *)a, *(const char *const *)b);
}


static int benchmark_one(const char *dir, const char *fname) {
  char path[512];
  snprintf(path, sizeof(path), "%s/%s", dir, fname);

  Grid grid = load_sudoku_from_file(path);
  if (grid == NULL) {
    printf("  %-28s LOAD-FAILED\n", fname);
    return 1;
  }

  int suppositions = 0;
  clock_t t0 = clock();
  char solved = solve_with_stats(grid, &suppositions);
  double dt_ms = 1000.0 * (double)(clock() - t0) / CLOCKS_PER_SEC;

  printf("  %-28s %-5s  %8.2f ms  ded=%5d  sup=%3d  filled=%2d/81\n", fname,
         solved ? "OK" : "STUCK", dt_ms, deduction_count, suppositions,
         grid_filled_count(grid));

  delete_grid(grid);
  return 0;
}

/* --benchmark mode: solve every *.txt grid in `dir` and report the timing,
   deduction and supposition counts (subject section 3.8.3). */
static int run_benchmark(const char *dir) {
  DIR *d = opendir(dir);
  if (d == NULL) {
    fprintf(stderr, "Error: cannot open directory '%s'\n", dir);
    return 1;
  }

  char *names[256];
  int n = 0;
  struct dirent *e;
  while ((e = readdir(d)) != NULL && n < 256) {
    const char *dot = strrchr(e->d_name, '.');
    if (dot == NULL || strcmp(dot, ".txt") != 0)
      continue;
    size_t len = strlen(e->d_name) + 1;
    names[n] = malloc(len);
    if (names[n] == NULL)
      break;
    memcpy(names[n], e->d_name, len);
    n++;
  }
  closedir(d);
  qsort(names, (size_t)n, sizeof(names[0]), cmp_strings);

  printf("Benchmark on '%s' (%d grids)\n", dir, n);
  printf("  %-28s %-5s  %11s  %-9s  %-7s  %s\n", "grid", "state", "time", "ded",
         "sup", "filled");

  clock_t t_all = clock();
  for (int i = 0; i < n; i++) {
    benchmark_one(dir, names[i]);
    free(names[i]);
  }
  double dt_all = 1000.0 * (double)(clock() - t_all) / CLOCKS_PER_SEC;
  printf("\nTotal: %.2f ms\n", dt_all);
  return 0;
}

int main(int argc, char **argv) {
  /* --benchmark short-circuits the ncurses UI entirely. */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-benchmark") == 0 ||
        strcmp(argv[i], "--benchmark") == 0) {
      const char *dir = (i + 1 < argc) ? argv[i + 1] : "tables";
      return run_benchmark(dir);
    }
  }

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