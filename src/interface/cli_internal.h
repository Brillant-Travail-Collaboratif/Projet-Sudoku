#ifndef CLI_INTERNAL_H
#define CLI_INTERNAL_H

#include "benchmark.h"
#include "cli.h"

#include "../file/grid_file.h"
#include "../generator/difficulty.h"
#include "../generator/generator.h"
#include "../grid/grid.h"
#include "../solver/solver.h"
#include "../sudoku_types/type_difficulty.h"
#include "../sudoku_types/type_grid.h"
#include "../sudoku_types/type_history.h"
#include "display.h"
#include "tui.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct CliOptions {
  Difficulty difficulty;
  unsigned int seed;
  char *loadFile;
  char *writeFilePath;
  const char *benchmarkDirectory;
  char verbose;
  char interactive;
  char benchmark;
  char help;
  char hasDifficulty;
  char hasSeed;
} CliOptions;

SCREEN *start_cli_curses(void);
void end_cli_curses(SCREEN *screen, CliOptions *options);
char *make_table_path(const char *path);
void free_options(CliOptions *options);
int parse_options(int argc, char **argv, CliOptions *options);
int compare_strings(const void *left, const void *right);
int benchmark_one(const char *directory, const char *fileName);
void solve_and_show(Grid grid, char verbose);
Grid prepare_grid(const CliOptions *options);
int run_grid_mode(CliOptions *options);
void print_usage(void);

#endif /* CLI_INTERNAL_H */
