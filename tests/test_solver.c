#include "../src/grid/grid.h"
#include "../src/solver/solver.h"
#include <CUnit/CUnit.h>

static const char k_solution[81] = {
    5, 3, 4, 6, 7, 8, 9, 1, 2, 6, 7, 2, 1, 9, 5, 3, 4, 8, 1, 9, 8,
    3, 4, 2, 5, 6, 7, 8, 5, 9, 7, 6, 1, 4, 2, 3, 4, 2, 6, 8, 5, 3,
    7, 9, 1, 7, 1, 3, 9, 2, 4, 8, 5, 6, 9, 6, 1, 5, 3, 7, 2, 8, 4,
    2, 8, 7, 4, 1, 9, 6, 3, 5, 3, 4, 5, 2, 8, 6, 1, 7, 9,
};

static Grid grid_from(const char cells[81]) {
  Grid g = create_grid();
  for (int i = 0; i < 81; i++)
    if (cells[i] != 0)
      set_grid_value_raw(g, (i % 9) + 1, (i / 9) + 1, cells[i]);
  return g;
}

static void test_is_grid_valid_detects_duplicate(void) {
  Grid g = create_grid();
  set_grid_value_raw(g, 1, 1, 5);
  set_grid_value_raw(g, 2, 1, 5);
  CU_ASSERT_EQUAL(is_grid_valid(g), 0);
  CU_ASSERT_EQUAL(is_grid_valid(NULL), 0);
  delete_grid(g);
}

static void test_solve_complete_grid(void) {
  char puzzle[81];
  for (int i = 0; i < 81; i++)
    puzzle[i] = k_solution[i];
  puzzle[0] = puzzle[40] = puzzle[80] = puzzle[7] = 0;

  Grid g = grid_from(puzzle);
  int sup = 0;
  CU_ASSERT_EQUAL(solve_with_stats(g, &sup), 1);
  CU_ASSERT_EQUAL(grid_filled_count(g), 81);
  for (int i = 0; i < 81; i++)
    CU_ASSERT_EQUAL(g->allTiles[i].value, k_solution[i]);
  CU_ASSERT_EQUAL(sup, 0);
  delete_grid(g);
}

static void test_solve_null(void) { CU_ASSERT_EQUAL(solve(NULL), 0); }

int register_solver_tests(void) {
  CU_pSuite suite = CU_add_suite("solver", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "is_grid_valid duplicate",
                  test_is_grid_valid_detects_duplicate) == NULL ||
      CU_add_test(suite, "solve complete grid", test_solve_complete_grid) ==
          NULL ||
      CU_add_test(suite, "solve NULL", test_solve_null) == NULL)
    return 1;
  return 0;
}
