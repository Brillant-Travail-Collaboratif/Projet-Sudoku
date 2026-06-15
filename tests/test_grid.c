#include "../src/grid/grid.h"
#include <CUnit/CUnit.h>

static void test_create_grid_starts_empty(void) {
  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  CU_ASSERT_EQUAL(grid_filled_count(grid), 0);

  for (unsigned char y = 1; y <= 9; y++)
    for (unsigned char x = 1; x <= 9; x++) {
      CU_ASSERT_EQUAL(get_grid_value_xy(grid, x, y), 0);
      char *p = get_grid_possibles_xy(grid, x, y);
      CU_ASSERT_PTR_NOT_NULL_FATAL(p);
      for (int d = 0; d < 9; d++)
        CU_ASSERT_EQUAL(p[d], 1);
    }
  delete_grid(grid);
}

static void test_set_get_value(void) {
  Grid grid = create_grid();
  CU_ASSERT_EQUAL(set_grid_value_xy(grid, 3, 4, 7, 0), 0);
  CU_ASSERT_EQUAL(get_grid_value_xy(grid, 3, 4), 7);
  CU_ASSERT_EQUAL(grid_filled_count(grid), 1);

  char *p = get_grid_possibles_xy(grid, 3, 4);
  CU_ASSERT_PTR_NOT_NULL_FATAL(p);
  CU_ASSERT_EQUAL(p[6], 1);
  CU_ASSERT_EQUAL(p[0], 0);
  delete_grid(grid);
}

static void test_value_bounds_are_rejected(void) {
  Grid grid = create_grid();

  CU_ASSERT_EQUAL(set_grid_value_xy(grid, 0, 1, 5, 0), 1);
  CU_ASSERT_EQUAL(set_grid_value_xy(grid, 10, 1, 5, 0), 1);
  CU_ASSERT_EQUAL(set_grid_value_xy(grid, 1, 0, 5, 0), 1);
  CU_ASSERT_EQUAL(set_grid_value_xy(grid, 1, 1, 10, 0), 1);
  CU_ASSERT_EQUAL(set_grid_value_xy(NULL, 1, 1, 5, 0), 1);
  CU_ASSERT_EQUAL(get_grid_value_xy(grid, 0, 1), 0);
  CU_ASSERT_EQUAL(get_grid_value_xy(grid, 10, 1), 0);
  CU_ASSERT_EQUAL(grid_filled_count(grid), 0);
  delete_grid(grid);
}

static void test_set_raw_does_not_journal(void) {
  Grid grid = create_grid();
  CU_ASSERT_EQUAL(set_grid_value_raw(grid, 1, 1, 4), 0);
  CU_ASSERT_EQUAL(get_grid_value_xy(grid, 1, 1), 4);
  char *p = get_grid_possibles_xy(grid, 1, 1);
  CU_ASSERT_EQUAL(p[3], 1);
  CU_ASSERT_EQUAL(p[0], 0);
  delete_grid(grid);
}

static void test_clone_is_independent(void) {
  Grid grid = create_grid();
  set_grid_value_xy(grid, 1, 1, 9, 0);
  Grid clone = clone_grid_values(grid);
  CU_ASSERT_PTR_NOT_NULL_FATAL(clone);
  CU_ASSERT_EQUAL(get_grid_value_xy(clone, 1, 1), 9);

  set_grid_value_xy(clone, 2, 2, 3, 0);
  CU_ASSERT_EQUAL(get_grid_value_xy(grid, 2, 2), 0);
  delete_grid(clone);
  delete_grid(grid);
}

static void test_null_safety(void) {
  CU_ASSERT_EQUAL(grid_filled_count(NULL), 0);
  CU_ASSERT_EQUAL(get_grid_value_xy(NULL, 1, 1), 0);
  CU_ASSERT_PTR_NULL(get_grid_possibles_xy(NULL, 1, 1));
  CU_ASSERT_PTR_NULL(clone_grid_values(NULL));
  delete_grid(NULL);
}

int register_grid_tests(void) {
  CU_pSuite suite = CU_add_suite("grid", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "create starts empty",
                  test_create_grid_starts_empty) == NULL ||
      CU_add_test(suite, "set/get value", test_set_get_value) == NULL ||
      CU_add_test(suite, "out-of-bounds rejected",
                  test_value_bounds_are_rejected) == NULL ||
      CU_add_test(suite, "raw set", test_set_raw_does_not_journal) == NULL ||
      CU_add_test(suite, "clone independent", test_clone_is_independent) ==
          NULL ||
      CU_add_test(suite, "NULL safety", test_null_safety) == NULL)
    return 1;
  return 0;
}
