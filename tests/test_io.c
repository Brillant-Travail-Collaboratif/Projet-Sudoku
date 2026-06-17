#include "../src/grid/grid.h"
#include "../src/io/read.h"
#include <CUnit/CUnit.h>
#include <stdio.h>

static const char *k_tmp = "/tmp/sudoku_io_test.txt";

static void write_file(const char *path, const char *content) {
  FILE *f = fopen(path, "w");
  CU_ASSERT_PTR_NOT_NULL_FATAL(f);
  fputs(content, f);
  fclose(f);
}

static void test_load_valid(void) {
  write_file(k_tmp, "5 3 ? ? 7 ? ? ? ?\n"
                    "6 ? ? 1 9 5 ? ? ?\n"
                    "? 9 8 ? ? ? ? 6 ?\n"
                    "8 ? ? ? 6 ? ? ? 3\n"
                    "4 ? ? 8 ? 3 ? ? 1\n"
                    "7 ? ? ? 2 ? ? ? 6\n"
                    "? 6 ? ? ? ? 2 8 ?\n"
                    "? ? ? 4 1 9 ? ? 5\n"
                    "? ? ? ? 8 ? ? 7 9\n");
  Grid g = load_sudoku_from_file(k_tmp);
  CU_ASSERT_PTR_NOT_NULL_FATAL(g);
  CU_ASSERT_EQUAL(get_grid_value_xy(g, 1, 1), 5);
  CU_ASSERT_EQUAL(get_grid_value_xy(g, 2, 1), 3);
  CU_ASSERT_EQUAL(get_grid_value_xy(g, 5, 1), 7);
  CU_ASSERT_EQUAL(get_grid_value_xy(g, 3, 1), 0); /* a '?' */
  CU_ASSERT_EQUAL(grid_filled_count(g), 30);
  delete_grid(g);
}

static void test_save_roundtrip(void) {
  write_file(k_tmp, "1 ? 3 ? 5 ? 7 ? 9\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n"
                    "? ? ? ? ? ? ? ? ?\n");
  Grid g = load_sudoku_from_file(k_tmp);
  CU_ASSERT_PTR_NOT_NULL_FATAL(g);

  const char *out = "/tmp/sudoku_io_out.txt";
  CU_ASSERT_EQUAL(save_sodoku_to_file(out, g), 0);

  Grid back = load_sudoku_from_file(out);
  CU_ASSERT_PTR_NOT_NULL_FATAL(back);
  for (unsigned char y = 1; y <= 9; y++)
    for (unsigned char x = 1; x <= 9; x++)
      CU_ASSERT_EQUAL(get_grid_value_xy(g, x, y),
                      get_grid_value_xy(back, x, y));
  delete_grid(back);
  delete_grid(g);
}

static void test_missing_and_null(void) {
  CU_ASSERT_PTR_NULL(load_sudoku_from_file("/tmp/does_not_exist_42.txt"));
  CU_ASSERT_PTR_NULL(load_sudoku_from_file(NULL));
  CU_ASSERT_EQUAL(save_sodoku_to_file(NULL, NULL), -1);
}

static void test_malformed_and_save_errors(void) {
  write_file(k_tmp, "1 ? 3\n");
  CU_ASSERT_PTR_NULL(load_sudoku_from_file(k_tmp));

  Grid g = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(g);
  CU_ASSERT_EQUAL(save_sodoku_to_file(NULL, g), -1);
  CU_ASSERT_EQUAL(save_sodoku_to_file("/tmp/sudoku_missing_dir/out.txt", g),
                  -1);
  CU_ASSERT_EQUAL(save_sodoku_to_file(k_tmp, NULL), -1);
  delete_grid(g);
}

int register_io_tests(void) {
  CU_pSuite suite = CU_add_suite("io", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "load valid", test_load_valid) == NULL ||
      CU_add_test(suite, "save roundtrip", test_save_roundtrip) == NULL ||
      CU_add_test(suite, "missing/NULL", test_missing_and_null) == NULL ||
      CU_add_test(suite, "malformed/save errors",
                  test_malformed_and_save_errors) == NULL)
    return 1;
  return 0;
}
