#include "cli_internal.h"

SCREEN *start_cli_curses(void) {
  SCREEN *screen = newterm(NULL, stdout, stdin);
  if (screen == NULL)
    return NULL;
  set_term(screen);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  return screen;
}

void end_cli_curses(SCREEN *screen, CliOptions *options) {
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

  char *fullPath = malloc(strlen(path) + strlen(prefix) + 1);
  if (fullPath == NULL)
    return NULL;
  sprintf(fullPath, "%s%s", prefix, path);
  return fullPath;
}

void free_options(CliOptions *options) {
  if (options == NULL)
    return;
  free(options->loadFile);
  options->loadFile = NULL;
}

int compare_strings(const void *left, const void *right) {
  return strcmp(*(const char *const *)left, *(const char *const *)right);
}

void solve_and_show(Grid grid, char verbose) {
  display_values(grid);

  int guesses = 0;
  char solved = solve_with_stats(grid, &guesses);

  display_values(grid);
  printf("%s\n", solved ? "Solved" : "Could not fully solve");
  if (verbose)
    printf("Guesses: %d   Deductions: %d\n", guesses, deductionCount);
}

Grid prepare_grid(const CliOptions *options) {
  if (options->loadFile != NULL)
    return load_grid_from_file(options->loadFile);

  if (options->interactive) {
    printw("Generating %s sudoku with seed %u, please wait ...\n",
           get_difficulty_name(options->difficulty), options->seed);
    refresh();
  } else {
    printf("Generating %s sudoku with seed %u, please wait ...\n",
           get_difficulty_name(options->difficulty), options->seed);
  }
  return generate_sudoku(options->difficulty, options->seed);
}

int run_grid_mode(CliOptions *options) {
  SCREEN *screen = NULL;

  if (options->interactive) {
    screen = start_cli_curses();
    if (screen == NULL)
      return 1;
  }

  Grid grid = prepare_grid(options);
  if (grid == NULL) {
    if (screen != NULL)
      end_cli_curses(screen, options);
    if (options->loadFile != NULL) {
      fprintf(stderr, "Could not load sudoku from '%s'\n", options->loadFile);
    } else {
      fprintf(stderr, "Could not generate a %s sudoku with seed %u\n",
              get_difficulty_name(options->difficulty), options->seed);
    }
    return 1;
  }

  if (options->interactive)
    start_grid_tui(&grid);
  else
    solve_and_show(grid, options->verbose);

  if (options->writeFilePath != NULL) {
    if (write_grid_to_file(options->writeFilePath, grid)) {
      fprintf(stderr, "Could not write grid to '%s'\n",
              options->writeFilePath);
      delete_grid(grid);
      if (screen != NULL)
        end_cli_curses(screen, options);
      return 1;
    }
  }

  delete_grid(grid);
  if (screen != NULL)
    end_cli_curses(screen, options);
  return 0;
}

void print_usage(void) {
  fprintf(
      stderr,
      "Usage: Projet_Sudoku [options]\n"
      "  -i, -interactive          edit the grid in the TUI\n"
      "  -l, -load FILE            load a grid from FILE\n"
      "  -v, -verbose              show solver statistics\n"
      "  -b, -benchmark [DIR]      solve every *.txt in DIR (default tables)\n"
      "  -g, -generate LEVEL       generate a LEVEL difficulty\n"
      "  -s, -seed NUMBER          set a seed for the random number generator\n"
      "  -w, -write FILE           write grid to file\n"
      "  -h, -help                 show this help\n");
}

int parse_options(int argc, char **argv, CliOptions *options) {
  *options = (CliOptions){BASIC, 1u, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0};

  for (int i = 1; i < argc; i++) {
    Difficulty parsed;
    char *end = NULL;

    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
      options->help = 1;
      return 1;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-verbose") == 0) {
      options->verbose = 1;
    } else if (strcmp(argv[i], "-i") == 0 ||
               strcmp(argv[i], "-interactive") == 0) {
      options->interactive = 1;
    } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "-load") == 0) {
      if (options->loadFile != NULL) {
        fprintf(stderr, "Error: load file was provided more than once\n");
        return 0;
      }
      if (i + 1 >= argc || argv[i + 1][0] == '-') {
        fprintf(stderr, "Error: %s requires a file\n", argv[i]);
        return 0;
      }
      options->loadFile = make_table_path(argv[++i]);
      if (options->loadFile == NULL) {
        fprintf(stderr, "Error: could not allocate load file path\n");
        return 0;
      }
    } else if (strcmp(argv[i], "-b") == 0 ||
               strcmp(argv[i], "-benchmark") == 0) {
      options->benchmark = 1;
      if (i + 1 < argc && argv[i + 1][0] != '-')
        options->benchmarkDirectory = argv[++i];
    } else if (strcmp(argv[i], "-g") == 0 ||
               strcmp(argv[i], "-generate") == 0) {
      if (i + 1 < argc && argv[i + 1][0] != '-' &&
          parse_difficulty(argv[++i], &parsed)) {
        options->difficulty = parsed;
        options->hasDifficulty = 1;
      } else {
        fprintf(stderr, "Error: %s requires a difficulty\n", argv[i]);
        return 0;
      }
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-seed") == 0) {
      if (i + 1 < argc && argv[i + 1][0] != '-') {
        errno = 0;
        unsigned long seed = strtoul(argv[++i], &end, 10);
        if (errno != ERANGE && end != argv[i] && *end == '\0' &&
            seed <= UINT_MAX) {
          options->seed = (unsigned int)seed;
          options->hasSeed = 1;
        } else {
          fprintf(stderr, "Error: %s requires a number\n", argv[i]);
          return 0;
        }
      } else {
        fprintf(stderr, "Error: %s requires a number\n", argv[i]);
        return 0;
      }
    } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "-write") == 0) {
      if (options->writeFilePath != NULL) {
        fprintf(stderr, "Error: write file was provided more than once\n");
        return 0;
      }
      if (i + 1 < argc && argv[i + 1][0] != '-') {
        options->writeFilePath = argv[++i];
      } else {
        fprintf(stderr, "Error: %s requires a file\n", argv[i]);
        return 0;
      }
    } else {
      fprintf(stderr, "Error: unsupported argument '%s'\n", argv[i]);
      return 0;
    }
  }

  if (options->benchmark &&
      (options->interactive || options->loadFile != NULL ||
       options->hasDifficulty || options->hasSeed ||
       options->writeFilePath)) {
    fprintf(stderr,
            "Error: benchmark mode can only be combined with verbose mode\n");
    return 0;
  }

  if (options->loadFile != NULL &&
      (options->benchmark || options->hasDifficulty || options->hasSeed)) {
    fprintf(stderr, "Error: load mode can only be combined with verbose or "
                    "interactive mode\n");
    return 0;
  }

  return 1;
}

int benchmark_one(const char *directory, const char *fileName) {
  char path[512];
  snprintf(path, sizeof(path), "%s/%s", directory, fileName);

  Grid grid = load_grid_from_file(path);
  if (grid == NULL) {
    printf("  %-28s LOAD-FAILED\n", fileName);
    return 1;
  }

  int guesses = 0;
  clock_t startTime = clock();
  char solved = solve_with_stats(grid, &guesses);
  double elapsedMilliseconds =
      1000.0 * (double)(clock() - startTime) / CLOCKS_PER_SEC;

  printf("  %-28s %-5s  %8.2f ms  ded=%5d  guesses=%3d  filled=%2d/81\n",
         fileName, solved ? "OK" : "STUCK", elapsedMilliseconds,
         deductionCount, guesses, count_filled_cells(grid));

  delete_grid(grid);
  return 0;
}

int run_benchmark(const char *directory) {
  const char *prefix = (strchr(directory, '/') != NULL) ? "" : "../";
  char *fullPath = malloc(strlen(directory) + strlen(prefix) + 1);
  if (fullPath == NULL)
    return 1;
  sprintf(fullPath, "%s%s", prefix, directory);

  DIR *directoryStream = opendir(fullPath);
  if (directoryStream == NULL) {
    fprintf(stderr, "Error: cannot open directory '%s'\n", fullPath);
    free(fullPath);
    return 1;
  }

  char *fileNames[256];
  int fileCount = 0;
  struct dirent *entry;
  while ((entry = readdir(directoryStream)) != NULL && fileCount < 256) {
    const char *extension = strrchr(entry->d_name, '.');
    if (extension == NULL || strcmp(extension, ".txt") != 0)
      continue;
    size_t length = strlen(entry->d_name) + 1;
    fileNames[fileCount] = malloc(length);
    if (fileNames[fileCount] == NULL)
      break;
    memcpy(fileNames[fileCount], entry->d_name, length);
    fileCount++;
  }
  closedir(directoryStream);
  qsort(fileNames, (size_t)fileCount, sizeof(fileNames[0]), compare_strings);

  printf("Benchmark on '%s' (%d grids)\n", directory, fileCount);
  printf("  %-28s %-5s  %11s  %-9s  %-11s  %s\n", "grid", "state",
         "time", "ded", "guesses", "filled");

  clock_t totalStart = clock();
  for (int i = 0; i < fileCount; i++) {
    benchmark_one(fullPath, fileNames[i]);
    free(fileNames[i]);
  }
  double totalMilliseconds =
      1000.0 * (double)(clock() - totalStart) / CLOCKS_PER_SEC;
  printf("\nTotal: %.2f ms\n", totalMilliseconds);
  free(fullPath);
  return 0;
}

int run_cli(int argc, char **argv) {
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
    const char *directory = (options.benchmarkDirectory != NULL)
                                ? options.benchmarkDirectory
                                : "tables";
    result = run_benchmark(directory);
  } else {
    result = run_grid_mode(&options);
  }

  free_options(&options);
  return result;
}
