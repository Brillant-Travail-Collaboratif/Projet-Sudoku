#include "tui_internal.h"

char *make_tui_table_path(const char *path) {
  const char *prefix = "../tables/";

  if (path == NULL || path[0] == '\0')
    return NULL;
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

void set_tui_message(char *message, size_t size, const char *text) {
  if (message == NULL || size == 0)
    return;
  snprintf(message, size, "%s", text == NULL ? "" : text);
}

void move_tui_cursor(unsigned char x, unsigned char y) {
  unsigned char mvx = x * 2;
  if (x >= 4)
    mvx += 2;
  if (x >= 7)
    mvx += 2;

  unsigned char mvy = y;
  if (y >= 4)
    mvy++;
  if (y >= 7)
    mvy++;

  move(mvy, mvx);
}

void draw_tui(Grid grid, const char *message, unsigned char x,
              unsigned char y) {
  display_values_curses(grid);
  printw("Arrows move | 1-9 set | ? clear | s solve | l load | g generate\n");
  printw("w write | b benchmark | q quit\n");
  if (message != NULL && message[0] != '\0')
    printw("%s\n", message);
  refresh();
  move_tui_cursor(x, y);
}

char read_tui_line(const char *prompt, char *buffer, size_t size) {
  int row = 0;
  int col = 0;

  if (buffer == NULL || size == 0)
    return 0;

  getmaxyx(stdscr, row, col);
  (void)col;
  move(row - 2, 0);
  clrtoeol();
  printw("%s", prompt);
  echo();
  curs_set(1);
  int result = getnstr(buffer, (int)size - 1);
  noecho();
  buffer[size - 1] = '\0';
  return result == OK && buffer[0] != '\0';
}

char load_tui_grid(Grid *grid, const char *path) {
  char *fullpath = make_tui_table_path(path);
  if (fullpath == NULL)
    return 0;

  Grid loaded = load_grid_from_file(fullpath);
  free(fullpath);
  if (loaded == NULL)
    return 0;

  delete_grid(*grid);
  *grid = loaded;
  return 1;
}

char generate_tui_grid(Grid *grid, Difficulty difficulty, unsigned int seed) {
  Grid generated = generate_sudoku(difficulty, seed);
  if (generated == NULL)
    return 0;

  delete_grid(*grid);
  *grid = generated;
  return 1;
}

char solve_tui_grid(Grid grid, char *message, size_t size) {
  char solved = solve(grid);
  set_tui_message(message, size,
                  solved ? "Grid solved" : "Could not fully solve grid");
  return solved;
}

void save_tui_grid(Grid grid, const char *path, char *message, size_t size) {
  if (write_grid_to_file(path, grid) == 0)
    set_tui_message(message, size, "Grid saved");
  else
    set_tui_message(message, size, "Could not save grid");
}

void run_tui_benchmark(const char *dir) {
  def_prog_mode();
  endwin();
  run_benchmark((dir == NULL || dir[0] == '\0') ? "tables" : dir);
  printf("Press Enter to return to the TUI...");
  fflush(stdout);
  int ch = 0;
  while ((ch = getchar()) != '\n' && ch != EOF) {
  }
  reset_prog_mode();
  refresh();
}

void start_grid_tui(Grid *grid) {
  char message[TUI_MESSAGE_SIZE] = "";
  unsigned char x = 1;
  unsigned char y = 1;

  draw_tui(*grid, message, x, y);

  int ch;
  while ((ch = getch()) != 'q' && ch != '\n') {
    if (ch == KEY_UP) {
      if (y > 1)
        y--;
    } else if (ch == KEY_DOWN) {
      if (y < NUMBER_OF_POSSIBLE)
        y++;
    } else if (ch == KEY_LEFT) {
      if (x > 1)
        x--;
    } else if (ch == KEY_RIGHT) {
      if (x < NUMBER_OF_POSSIBLE)
        x++;
    } else if (ch == '?') {
      set_grid_value_xy(*grid, x, y, 0, 0);
      set_tui_message(message, sizeof(message), "");
    } else if (ch == 's') {
      solve_tui_grid(*grid, message, sizeof(message));
    } else if (ch == 'l') {
      char path[TUI_INPUT_SIZE];
      if (read_tui_line("Load file: ", path, sizeof(path))) {
        if (load_tui_grid(grid, path))
          set_tui_message(message, sizeof(message), "Grid loaded");
        else
          set_tui_message(message, sizeof(message), "Could not load grid");
      }
    } else if (ch == 'g') {
      char level[TUI_INPUT_SIZE];
      char seedText[TUI_INPUT_SIZE];
      Difficulty difficulty;
      char *end = NULL;

      if (read_tui_line("Difficulty: ", level, sizeof(level)) &&
          parse_difficulty(level, &difficulty) &&
          read_tui_line("Seed: ", seedText, sizeof(seedText))) {
        unsigned long seed = strtoul(seedText, &end, 10);
        if (end != seedText && *end == '\0' && seed <= UINT_MAX &&
            generate_tui_grid(grid, difficulty, (unsigned int)seed)) {
          set_tui_message(message, sizeof(message), "Grid generated");
        } else {
          set_tui_message(message, sizeof(message), "Could not generate grid");
        }
      } else {
        set_tui_message(message, sizeof(message), "Invalid difficulty or seed");
      }
    } else if (ch == 'w') {
      char path[TUI_INPUT_SIZE];
      if (read_tui_line("Write to file: ", path, sizeof(path)))
        save_tui_grid(*grid, path, message, sizeof(message));
    } else if (ch == 'b') {
      char dir[TUI_INPUT_SIZE] = "";
      read_tui_line("Benchmark dir (empty = tables): ", dir, sizeof(dir));
      run_tui_benchmark(dir);
      set_tui_message(message, sizeof(message), "Benchmark finished");
    } else if (ch >= '0' && ch <= '9') {
      ch -= '0';
      if (ch > 0 && ch <= 9) {
        set_grid_value_xy(*grid, x, y, ch, 0);
        set_tui_message(message, sizeof(message), "");
      }
    }

    draw_tui(*grid, message, x, y);
  }
}
