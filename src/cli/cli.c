#include "cli_internal.h"

SCREEN *cli_start_curses(void) {
  SCREEN *screen = newterm(NULL, stdout, stdin);
  if (screen == NULL)
    return NULL;
  set_term(screen);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  return screen;
}

void cli_end_curses(SCREEN *screen, CliOptions *options) {
  if (!options->interactive) {
    printw("Press any key to exit.");
    refresh();
    getch();
  }
  endwin();
  delscreen(screen);
}

char *make_table_path(const char *path) {
  const char *prefix = "../tables/";

  if (strchr(path, '/') != NULL) {
    size_t len = strlen(path) + 1;
    char *copy = malloc(len);
    if (copy == NULL)
      return NULL;
    memcpy(copy, path, len);
    return copy;
  }

  char *fullpath = malloc(strlen(path) + strlen(prefix) + 1);
  if (fullpath == NULL)
    return NULL;
  sprintf(fullpath, "%s%s", prefix, path);
  return fullpath;
}

void free_options(CliOptions *options) {
  if (options == NULL)
    return;
  free(options->file);
  options->file = NULL;
}

int cmp_strings(const void *a, const void *b) {
  return strcmp(*(const char *const *)a, *(const char *const *)b);
}

void solve_and_show(Grid grid, char verbose) {
  display_values(grid);
  refresh();

  int suppositions = 0;
  char solved = solve_with_stats(grid, &suppositions);

  display_values(grid);
  printw("%s\n", solved ? "Solved" : "Could not fully solve");
  if (verbose)
    printw("Suppositions: %d   Deductions: %d\n", suppositions,
           deduction_count);
  refresh();
}

Grid prepare_grid(const CliOptions *options) {
  if (options->file != NULL)
    return load_sudoku_from_file(options->file);

  printw("Generating %s sudoku with seed %u, please wait ...\n",
         difficulty_to_string(options->difficulty), options->seed);
  refresh();
  return generate_sudoku(options->difficulty, options->seed);
}

int run_grid_mode(CliOptions *options) {
  SCREEN *screen = cli_start_curses();
  if (screen == NULL)
    return 1;

  Grid grid = prepare_grid(options);
  if (grid == NULL) {
    cli_end_curses(screen, options);
    if (options->file != NULL) {
      fprintf(stderr, "Could not load sudoku from '%s'\n", options->file);
    } else {
      fprintf(stderr, "Could not generate a %s sudoku with seed %u\n",
              difficulty_to_string(options->difficulty), options->seed);
    }
    return 1;
  }

  if (options->interactive)
    start_grid_tui(grid);
  else
    solve_and_show(grid, options->verbose);

  delete_grid(grid);
  cli_end_curses(screen, options);
  return 0;
}

void print_usage(void) {
  fprintf(
      stderr,
      "Usage: Projet_Sudoku [options] [DIFFICULTY] [SEED]\n"
      "  -i, -interactive          edit the grid in the TUI\n"
      "  -l, -load FILE            load a grid from FILE\n"
      "  -v, -verbose              show solver statistics\n"
      "  -b, -benchmark [DIR]      solve every *.txt in DIR (default tables)\n"
      "  -h, -help                 show this help\n");
}

int parse_options(int argc, char **argv, CliOptions *options) {
  *options = (CliOptions){BASIC, 1u, NULL, NULL, 0, 0, 0, 0, 0, 0};

  for (int i = 1; i < argc; i++) {
    Difficulty parsed;
    char *end = NULL;

    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
      options->help = 1;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-verbose") == 0) {
      options->verbose = 1;
    } else if (strcmp(argv[i], "-i") == 0 ||
               strcmp(argv[i], "-interactive") == 0) {
      options->interactive = 1;
    } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "-load") == 0) {
      if (options->file != NULL) {
        fprintf(stderr, "Error: load file was provided more than once\n");
        return 0;
      }
      if (i + 1 >= argc || argv[i + 1][0] == '-') {
        fprintf(stderr, "Error: %s requires a file\n", argv[i]);
        return 0;
      }
      options->file = make_table_path(argv[++i]);
      if (options->file == NULL) {
        fprintf(stderr, "Error: could not allocate load file path\n");
        return 0;
      }
    } else if (strcmp(argv[i], "-b") == 0 ||
               strcmp(argv[i], "-benchmark") == 0) {
      options->benchmark = 1;
      if (i + 1 < argc && argv[i + 1][0] != '-')
        options->benchmark_dir = argv[++i];
    } else if (parse_difficulty(argv[i], &parsed)) {
      options->difficulty = parsed;
      options->has_difficulty = 1;
    } else {
      unsigned long seed = strtoul(argv[i], &end, 10);
      if (end != argv[i] && *end == '\0') {
        options->seed = (unsigned int)seed;
        options->has_seed = 1;
      } else {
        fprintf(stderr, "Error: unsupported argument '%s'\n", argv[i]);
        return 0;
      }
    }
  }

  if (options->help)
    return 1;

  if (options->benchmark &&
      (options->interactive || options->verbose || options->file != NULL ||
       options->has_difficulty || options->has_seed)) {
    fprintf(stderr, "Error: benchmark mode cannot be combined with solve or "
                    "interactive options\n");
    return 0;
  }

  return 1;
}

int benchmark_one(const char *dir, const char *fname) {
  char path[512];
  if (strchr(dir, '/') != NULL)
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
  else
    snprintf(path, sizeof(path), "../%s/%s", dir, fname);

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

int run_benchmark(const char *dir) {
  const char *prefix = (strchr(dir, '/') != NULL) ? "" : "../";
  char *fullpath = malloc(strlen(dir) + strlen(prefix) + 1);
  if (fullpath == NULL) {
    free(fullpath);
    return 1;
  }
  sprintf(fullpath, "%s%s", prefix, dir);

  DIR *d = opendir(fullpath);
  if (d == NULL) {
    fprintf(stderr, "Error: cannot open directory '%s'\n", fullpath);
    free(fullpath);
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
  free(fullpath);
  return 0;
}

int cli_run(int argc, char **argv) {
  CliOptions options;
  int result = 0;

  if (!parse_options(argc, argv, &options)) {
    print_usage();
    free_options(&options);
    return 1;
  }

  if (options.help) {
    print_usage();
  } else if (options.benchmark) {
    const char *dir =
        (options.benchmark_dir != NULL) ? options.benchmark_dir : "tables";
    result = run_benchmark(dir);
  } else {
    result = run_grid_mode(&options);
  }

  free_options(&options);
  return result;
}
