#include "../src/file/grid_file.h"
#include "../src/grid/grid.h"
#include "../src/solver/solver.h"
#include "../src/sudoku_types/type_subset.h"
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <time.h>

#ifndef TABLES_DIR
#define TABLES_DIR "tables"
#endif

#define PER_GRID_TIME_LIMIT_SECONDS 30.0

static const struct {
  const char *file;
  int mustSolve;
} TABLE_CASES[] = {
    {"trivial_table_1.txt", 1},      {"trivial_table_2.txt", 1},
    {"basic_table_1.txt", 1},        {"intermediate_table_1.txt", 1},
    {"intermediate_table_2.txt", 1}, {"difficile_table_1.txt", 1},
    {"expert_table_1.txt", 1},       {"expert_table_2.txt", 0},
};

static int has_no_duplicates(Grid grid) {
  for (int subsetIndex = 0; subsetIndex < SUBSET_COUNT; subsetIndex++) {
    int seen[9] = {0};
    for (int cellIndex = 0; cellIndex < 9; cellIndex++) {
      int value = grid->subsets.items[subsetIndex][cellIndex]->value;
      if (value == 0)
        continue;
      if (seen[value - 1])
        return 0;
      seen[value - 1] = 1;
    }
  }
  return 1;
}

static void test_solve_all_tables(void) {
  char path[512];
  for (size_t i = 0; i < sizeof(TABLE_CASES) / sizeof(TABLE_CASES[0]); i++) {
    snprintf(path, sizeof(path), "../%s/%s", TABLES_DIR, TABLE_CASES[i].file);
    Grid grid = load_grid_from_file(path);
    CU_ASSERT_PTR_NOT_NULL_FATAL(grid);

    clock_t startTime = clock();
    char solved = solve(grid);
    double elapsedSeconds = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    if (TABLE_CASES[i].mustSolve) {
      CU_ASSERT_TRUE(solved);
      CU_ASSERT_EQUAL(count_filled_cells(grid), 81);
    }
    CU_ASSERT_TRUE(has_no_duplicates(grid));
    CU_ASSERT_TRUE(elapsedSeconds < PER_GRID_TIME_LIMIT_SECONDS);

    delete_grid(grid);
  }
}

int register_tables_tests(void) {
  CU_pSuite suite = CU_add_suite("tables (integration)", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "solve every table in budget",
                  test_solve_all_tables) == NULL)
    return 1;
  return 0;
}
