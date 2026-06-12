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
    if(!options->interactive) {
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
    fprintf(stderr,
            "Usage: Projet_Sudoku [options] [DIFFICULTY] [SEED]\n"
            "  -i, -interactive          edit the grid in the TUI\n"
            "  -l, -load FILE            load a grid from FILE\n"
            "  -v, -verbose              show solver statistics\n"
            "  -b, -benchmark [DIR]      solve every *.txt in DIR (default tables)\n"
            "  -h, -help                 show this help\n");
}
