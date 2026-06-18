#ifndef CLI_INTERNAL_H
#define CLI_INTERNAL_H

#include "cli.h"

#include "../generator/difficulty.h"
#include "../generator/generator.h"
#include "../grid/grid.h"
#include "../io/display.h"
#include "../io/read.h"
#include "../solver/solver.h"
#include "../sudoku_types/type_difficulty.h"
#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_history.h"

#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct CliOptions {
  Difficulty difficulty;
  unsigned int seed;
  char *load_file;
  char *save_filepath;
  const char *benchmark_dir;
  char verbose;
  char interactive;
  char benchmark;
  char help;
  char has_difficulty;
  char has_seed;
} CliOptions;

SCREEN *cli_start_curses(void);
void cli_end_curses(SCREEN *screen, CliOptions *options);
char *make_table_path(const char *path);
void free_options(CliOptions *options);
int parse_options(int argc, char **argv, CliOptions *options);
int cmp_strings(const void *a, const void *b);
int benchmark_one(const char *dir, const char *fname);
int run_benchmark(const char *dir);
void solve_and_show(Grid grid, char verbose);
Grid prepare_grid(const CliOptions *options);
int run_grid_mode(CliOptions *options);
void print_usage(void);

#endif /* CLI_INTERNAL_H */
