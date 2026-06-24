#ifndef TUI_INTERNAL_H
#define TUI_INTERNAL_H

#include "benchmark.h"
#include "display_internal.h"
#include "tui.h"

#include "../file/grid_file.h"
#include "../generator/difficulty.h"
#include "../generator/generator.h"
#include "../solver/solver.h"

#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUI_MESSAGE_SIZE 160
#define TUI_INPUT_SIZE 256

char *make_tui_table_path(const char *path);
void set_tui_message(char *message, size_t size, const char *text);
void draw_tui(Grid grid, const char *message, unsigned char x, unsigned char y);
void move_tui_cursor(unsigned char x, unsigned char y);
char read_tui_line(const char *prompt, char *buffer, size_t size);
char load_tui_grid(Grid *grid, const char *path);
char generate_tui_grid(Grid *grid, Difficulty difficulty, unsigned int seed);
char solve_tui_grid(Grid grid, char *message, size_t size);
void save_tui_grid(Grid grid, const char *path, char *message, size_t size);
void run_tui_benchmark(const char *dir);

#endif /* TUI_INTERNAL_H */
