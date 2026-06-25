#include "../src/grid/grid.h"
#include "../src/subset/subset.h"
#include <CUnit/CUnit.h>

static SudokuCell *get_test_cell(Grid grid, int row, int column) {
  return &grid->cells[row * 9 + column];
}

static void test_row_subset(void) {
  Grid grid = create_grid();
  Subset subset = get_row_subset(grid, 2);
  CU_ASSERT_PTR_NOT_NULL_FATAL(subset);
  for (int column = 0; column < 9; column++)
    CU_ASSERT_PTR_EQUAL(subset[column], get_test_cell(grid, 2, column));
  delete_subset(subset);
  delete_grid(grid);
}

static void test_column_subset(void) {
  Grid grid = create_grid();
  Subset subset = get_column_subset(grid, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(subset);
  for (int row = 0; row < 9; row++)
    CU_ASSERT_PTR_EQUAL(subset[row], get_test_cell(grid, row, 5));
  delete_subset(subset);
  delete_grid(grid);
}

static void test_box_subset_numbering(void) {
  Grid grid = create_grid();

  Subset subset = get_box_subset(grid, 4);
  CU_ASSERT_PTR_NOT_NULL_FATAL(subset);
  CU_ASSERT_PTR_EQUAL(subset[0], get_test_cell(grid, 3, 3));
  CU_ASSERT_PTR_EQUAL(subset[8], get_test_cell(grid, 5, 5));
  delete_subset(subset);

  subset = get_box_subset(grid, 2);
  CU_ASSERT_PTR_EQUAL(subset[0], get_test_cell(grid, 0, 6));
  CU_ASSERT_PTR_EQUAL(subset[8], get_test_cell(grid, 2, 8));
  delete_subset(subset);
  delete_grid(grid);
}

static void test_build_all_subsets_covers_grid(void) {
  Grid grid = create_grid();

  CU_ASSERT_EQUAL(build_all_subsets(grid), 0);
  for (int i = 0; i < SUBSET_COUNT; i++)
    CU_ASSERT_PTR_NOT_NULL(grid->subsets.items[i]);
  delete_grid(grid);
}

static void test_out_of_range_returns_null(void) {
  Grid grid = create_grid();
  CU_ASSERT_PTR_NULL(get_row_subset(grid, -1));
  CU_ASSERT_PTR_NULL(get_row_subset(grid, 9));
  CU_ASSERT_PTR_NULL(get_row_subset(NULL, 0));
  CU_ASSERT_PTR_NULL(get_column_subset(grid, -1));
  CU_ASSERT_PTR_NULL(get_column_subset(grid, 9));
  CU_ASSERT_PTR_NULL(get_column_subset(NULL, 0));
  CU_ASSERT_PTR_NULL(get_box_subset(grid, -1));
  CU_ASSERT_PTR_NULL(get_box_subset(grid, 9));
  CU_ASSERT_PTR_NULL(get_box_subset(NULL, 0));
  delete_subset(NULL);
  delete_grid(grid);
}

static void test_build_and_free_all_subsets_errors(void) {
  CU_ASSERT_EQUAL(build_all_subsets(NULL), 1);
  free_all_subsets(NULL);

  Grid grid = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
  free_all_subsets(&grid->subsets);
  for (int i = 0; i < SUBSET_COUNT; i++)
    CU_ASSERT_PTR_NULL(grid->subsets.items[i]);
  CU_ASSERT_EQUAL(build_all_subsets(grid), 0);
  delete_grid(grid);
}

int register_subset_tests(void) {
  CU_pSuite suite = CU_add_suite("subset", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "row subset", test_row_subset) == NULL ||
      CU_add_test(suite, "column subset", test_column_subset) == NULL ||
      CU_add_test(suite, "box numbering", test_box_subset_numbering) ==
          NULL ||
      CU_add_test(suite, "build all subsets",
                  test_build_all_subsets_covers_grid) == NULL ||
      CU_add_test(suite, "out of range -> NULL",
                  test_out_of_range_returns_null) == NULL ||
      CU_add_test(suite, "build/free all subset errors",
                  test_build_and_free_all_subsets_errors) == NULL)
    return 1;
  return 0;
}
