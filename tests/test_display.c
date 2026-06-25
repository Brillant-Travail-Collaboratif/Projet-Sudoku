#include "../src/grid/grid.h"
#include "../src/interface/display.h"
#include "../src/interface/tui.h"
#include "../src/interface/tui_internal.h"
#include "../src/subset/subset.h"
#include <CUnit/CUnit.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

static int start_test_curses(FILE **input, FILE **output, SCREEN **screen) {
  *input = tmpfile();
  *output = tmpfile();
  if (*input == NULL || *output == NULL)
    return 0;

  *screen = newterm("xterm", *output, *input);
  if (*screen == NULL)
    return 0;

  set_term(*screen);
  return 1;
}

static void stop_test_curses(FILE *input, FILE *output, SCREEN *screen) {
  if (screen != NULL) {
    endwin();
    delscreen(screen);
  }
  if (input != NULL)
    fclose(input);
  if (output != NULL)
    fclose(output);
}

static void test_display_values_and_candidates(void) {
  FILE *input = NULL;
  FILE *output = NULL;
  SCREEN *screen = NULL;
  CU_ASSERT_TRUE_FATAL(start_test_curses(&input, &output, &screen));

  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  set_grid_value_raw(grid, 1, 1, 5);

  display_values(grid);
  display_values(NULL);
  display_candidates(grid);
  display_candidates(NULL);

  delete_grid(grid);
  stop_test_curses(input, output, screen);
}

static void test_start_grid_tui_quits(void) {
  FILE *input = NULL;
  FILE *output = NULL;
  SCREEN *screen = NULL;
  CU_ASSERT_TRUE_FATAL(start_test_curses(&input, &output, &screen));

  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  ungetch('q');
  start_grid_tui(&grid);

  delete_grid(grid);
  stop_test_curses(input, output, screen);
}

static void test_tui_reports_solver_failure(void) {
  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  set_grid_value_raw(grid, 1, 1, 5);
  set_grid_value_raw(grid, 2, 1, 5);

  char message[TUI_MESSAGE_SIZE] = "";
  CU_ASSERT_FALSE(solve_tui_grid(grid, message, sizeof(message)));
  CU_ASSERT_STRING_EQUAL(message, "Could not fully solve grid");

  delete_grid(grid);
}

static void test_display_subset(void) {
  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  set_grid_value_raw(grid, 1, 1, 4);

  Subset subset = get_row_subset(grid, 0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(subset);
  display_subset(subset);
  display_subset(NULL);

  delete_subset(subset);
  delete_grid(grid);
}

int register_display_tests(void) {
  CU_pSuite suite = CU_add_suite("display", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "values and candidates",
                  test_display_values_and_candidates) == NULL ||
      CU_add_test(suite, "TUI quits", test_start_grid_tui_quits) == NULL ||
      CU_add_test(suite, "TUI reports solver failure",
                  test_tui_reports_solver_failure) == NULL ||
      CU_add_test(suite, "subset", test_display_subset) == NULL)
    return 1;
  return 0;
}
