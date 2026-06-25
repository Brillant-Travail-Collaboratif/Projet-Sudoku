#include "../src/grid/grid.h"
#include "../src/solver/solver.h"
#include <CUnit/CUnit.h>

static const char SOLVED_GRID[81] = {
        5, 3, 4, 6, 7, 8, 9, 1, 2, 6, 7, 2, 1, 9, 5, 3, 4, 8, 1, 9, 8,
        3, 4, 2, 5, 6, 7, 8, 5, 9, 7, 6, 1, 4, 2, 3, 4, 2, 6, 8, 5, 3,
        7, 9, 1, 7, 1, 3, 9, 2, 4, 8, 5, 6, 9, 6, 1, 5, 3, 7, 2, 8, 4,
        2, 8, 7, 4, 1, 9, 6, 3, 5, 3, 4, 5, 2, 8, 6, 1, 7, 9,
};

static Grid create_grid_from_cells(const char cells[81]) {
  Grid grid = create_grid();
  for (int i = 0; i < 81; i++)
    if (cells[i] != 0)
      set_grid_value_raw(grid, (i % 9) + 1, (i / 9) + 1, cells[i]);
  return grid;
}

static void test_is_grid_valid_detects_duplicate(void) {
  Grid grid = create_grid();
  set_grid_value_raw(grid, 1, 1, 5);
  set_grid_value_raw(grid, 2, 1, 5);
  CU_ASSERT_EQUAL(is_grid_valid(grid), 0);
  CU_ASSERT_EQUAL(is_grid_valid(NULL), 0);
  delete_grid(grid);
}

static void test_is_grid_valid_accepts_empty_and_rejects_no_candidate(void) {
  Grid grid = create_grid();
  CU_ASSERT_EQUAL(is_grid_valid(grid), 1);

  char *candidates = get_grid_candidates_xy(grid, 1, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(candidates);
  for (int d = 0; d < 9; d++)
    candidates[d] = 0;
  CU_ASSERT_EQUAL(is_grid_valid(grid), 0);
  delete_grid(grid);
}

static void test_solve_complete_grid(void) {
  char puzzle[81];
  for (int i = 0; i < 81; i++)
    puzzle[i] = SOLVED_GRID[i];
  puzzle[0] = puzzle[40] = puzzle[80] = puzzle[7] = 0;

  Grid grid = create_grid_from_cells(puzzle);
  int guessCount = 0;
  CU_ASSERT_EQUAL(solve_with_stats(grid, &guessCount), 1);
  CU_ASSERT_EQUAL(count_filled_cells(grid), 81);
  for (int i = 0; i < 81; i++)
    CU_ASSERT_EQUAL(grid->cells[i].value, SOLVED_GRID[i]);
  CU_ASSERT_EQUAL(guessCount, 0);
  delete_grid(grid);
}

static void test_solve_null(void) { CU_ASSERT_EQUAL(solve(NULL), 0); }

static void test_solve_rejects_contradiction_and_null_stats(void) {
  Grid grid = create_grid();
  set_grid_value_raw(grid, 1, 1, 5);
  set_grid_value_raw(grid, 2, 1, 5);
  CU_ASSERT_EQUAL(solve(grid), 0);
  CU_ASSERT_EQUAL(solve_with_stats(NULL, NULL), 0);
  delete_grid(grid);
}

int register_solver_tests(void) {
  CU_pSuite suite = CU_add_suite("solver", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "is_grid_valid duplicate",
                  test_is_grid_valid_detects_duplicate) == NULL ||
      CU_add_test(suite, "is_grid_valid empty/no candidate",
                  test_is_grid_valid_accepts_empty_and_rejects_no_candidate) ==
      NULL ||
      CU_add_test(suite, "solve complete grid", test_solve_complete_grid) ==
      NULL ||
      CU_add_test(suite, "solve NULL", test_solve_null) == NULL ||
      CU_add_test(suite, "solve contradiction/null stats",
                  test_solve_rejects_contradiction_and_null_stats) == NULL)
    return 1;
  return 0;
}
