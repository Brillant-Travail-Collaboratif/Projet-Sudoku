#include "../src/grid/grid.h"
#include <CUnit/CUnit.h>

static void test_create_grid_starts_empty(void) {
  Grid grid = create_grid();

  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  CU_ASSERT_EQUAL(grid_filled_count(grid), 0);

  delete_grid(grid);
}

int register_grid_tests(void) {
  CU_pSuite suite = CU_add_suite("grid functions", NULL, NULL);
  if (suite == NULL)
    return 1;

  if (CU_add_test(suite, "create_grid starts empty",
                  test_create_grid_starts_empty) == NULL)
    return 1;

  return 0;
}