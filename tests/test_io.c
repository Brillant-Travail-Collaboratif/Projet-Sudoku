#include "../src/file/grid_file.h"
#include "../src/grid/grid.h"
#include <CUnit/CUnit.h>
#include <stdio.h>

static const char *TEST_FILE_PATH = "/tmp/sudoku_io_test.txt";

static void write_file(const char *path, const char *content) {
    FILE *file = fopen(path, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(file);
    fputs(content, file);
    fclose(file);
}

static void test_load_valid(void) {
    historyIndex = 0;
    write_file(TEST_FILE_PATH, "5 3 ? ? 7 ? ? ? ?\n"
                      "6 ? ? 1 9 5 ? ? ?\n"
                      "? 9 8 ? ? ? ? 6 ?\n"
                      "8 ? ? ? 6 ? ? ? 3\n"
                      "4 ? ? 8 ? 3 ? ? 1\n"
                      "7 ? ? ? 2 ? ? ? 6\n"
                      "? 6 ? ? ? ? 2 8 ?\n"
                      "? ? ? 4 1 9 ? ? 5\n"
                      "? ? ? ? 8 ? ? 7 9\n");
    Grid grid = load_grid_from_file(TEST_FILE_PATH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 1, 1), 5);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 2, 1), 3);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 5, 1), 7);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 3, 1), 0); /* a '?' */
    CU_ASSERT_EQUAL(count_filled_cells(grid), 30);
    CU_ASSERT_EQUAL(historyIndex, 0);
    delete_grid(grid);
}

static void test_save_roundtrip(void) {
    write_file(TEST_FILE_PATH, "1 ? 3 ? 5 ? 7 ? 9\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n");
    Grid grid = load_grid_from_file(TEST_FILE_PATH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(grid);

    const char *outputPath = "/tmp/sudoku_io_out.txt";
    CU_ASSERT_EQUAL(write_grid_to_file(outputPath, grid), 0);

    Grid reloadedGrid = load_grid_from_file(outputPath);
    CU_ASSERT_PTR_NOT_NULL_FATAL(reloadedGrid);
    for (unsigned char y = 1; y <= 9; y++)
        for (unsigned char x = 1; x <= 9; x++)
            CU_ASSERT_EQUAL(get_grid_value_xy(grid, x, y),
                            get_grid_value_xy(reloadedGrid, x, y));
    delete_grid(reloadedGrid);
    delete_grid(grid);
}

static void test_missing_and_null(void) {
    CU_ASSERT_PTR_NULL(load_grid_from_file("/tmp/does_not_exist_42.txt"));
    CU_ASSERT_PTR_NULL(load_grid_from_file(NULL));
    CU_ASSERT_EQUAL(write_grid_to_file(NULL, NULL), 1);
}

static void test_malformed_and_save_errors(void) {
    write_file(TEST_FILE_PATH, "1 ? 3\n");
    CU_ASSERT_PTR_NULL(load_grid_from_file(TEST_FILE_PATH));

    write_file(TEST_FILE_PATH, "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? x ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n");
    CU_ASSERT_PTR_NULL(load_grid_from_file(TEST_FILE_PATH));

    write_file(TEST_FILE_PATH, "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "? ? ? ? ? ? ? ? ?\n"
                      "1\n");
    CU_ASSERT_PTR_NULL(load_grid_from_file(TEST_FILE_PATH));

    Grid grid = create_grid();
    CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
    CU_ASSERT_EQUAL(write_grid_to_file(NULL, grid), 1);
    CU_ASSERT_EQUAL(
        write_grid_to_file("/tmp/sudoku_missing_dir/out.txt", grid), 1);
    CU_ASSERT_EQUAL(write_grid_to_file(TEST_FILE_PATH, NULL), 1);
    delete_grid(grid);
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
