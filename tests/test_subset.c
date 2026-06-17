#include "../src/grid/grid.h"
#include "../src/subset/subset.h"
#include <CUnit/CUnit.h>

static SudokuTile *cell(Grid g, int row, int col) {
  return &g->allTiles[row * 9 + col];
}

static void test_line_subset(void) {
  Grid g = create_grid();
  Subset s = get_line_subset(g, 2);
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  for (int c = 0; c < 9; c++)
    CU_ASSERT_PTR_EQUAL(s[c], cell(g, 2, c));
  delete_subset(s);
  delete_grid(g);
}

static void test_col_subset(void) {
  Grid g = create_grid();
  Subset s = get_col_subset(g, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  for (int r = 0; r < 9; r++)
    CU_ASSERT_PTR_EQUAL(s[r], cell(g, r, 5));
  delete_subset(s);
  delete_grid(g);
}

static void test_subsq_subset_numbering(void) {
  Grid g = create_grid();

  Subset s = get_subsq_subset(g, 4);
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  CU_ASSERT_PTR_EQUAL(s[0], cell(g, 3, 3));
  CU_ASSERT_PTR_EQUAL(s[8], cell(g, 5, 5));
  delete_subset(s);

  s = get_subsq_subset(g, 2);
  CU_ASSERT_PTR_EQUAL(s[0], cell(g, 0, 6));
  CU_ASSERT_PTR_EQUAL(s[8], cell(g, 2, 8));
  delete_subset(s);
  delete_grid(g);
}

static void test_build_all_subsets_covers_grid(void) {
  Grid g = create_grid();

  CU_ASSERT_EQUAL(build_all_subsets(g), 0);
  for (int i = 0; i < SUBSET_COUNT; i++)
    CU_ASSERT_PTR_NOT_NULL(g->allSubsets.subsets[i]);
  delete_grid(g);
}

static void test_out_of_range_returns_null(void) {
  Grid g = create_grid();
  CU_ASSERT_PTR_NULL(get_line_subset(g, -1));
  CU_ASSERT_PTR_NULL(get_line_subset(g, 9));
  CU_ASSERT_PTR_NULL(get_line_subset(NULL, 0));
  CU_ASSERT_PTR_NULL(get_col_subset(g, -1));
  CU_ASSERT_PTR_NULL(get_col_subset(g, 9));
  CU_ASSERT_PTR_NULL(get_col_subset(NULL, 0));
  CU_ASSERT_PTR_NULL(get_subsq_subset(g, -1));
  CU_ASSERT_PTR_NULL(get_subsq_subset(g, 9));
  CU_ASSERT_PTR_NULL(get_subsq_subset(NULL, 0));
  delete_subset(NULL);
  delete_grid(g);
}

static void test_build_and_free_all_subsets_errors(void) {
  CU_ASSERT_EQUAL(build_all_subsets(NULL), 1);
  free_all_subsets(NULL);

  Grid g = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(g);
  free_all_subsets(&g->allSubsets);
  for (int i = 0; i < SUBSET_COUNT; i++)
    CU_ASSERT_PTR_NULL(g->allSubsets.subsets[i]);
  CU_ASSERT_EQUAL(build_all_subsets(g), 0);
  delete_grid(g);
}

int register_subset_tests(void) {
  CU_pSuite suite = CU_add_suite("subset", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "line subset", test_line_subset) == NULL ||
      CU_add_test(suite, "col subset", test_col_subset) == NULL ||
      CU_add_test(suite, "box numbering", test_subsq_subset_numbering) ==
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
