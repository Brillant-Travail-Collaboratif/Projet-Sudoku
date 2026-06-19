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

#define PER_GRID_BUDGET_SEC 30.0

static const struct {
  const char *file;
  int must_solve;
} k_tables[] = {
    {"trivial_table_1.txt", 1},      {"trivial_table_2.txt", 1},
    {"basic_table_1.txt", 1},        {"intermediate_table_1.txt", 1},
    {"intermediate_table_2.txt", 1}, {"difficile_table_1.txt", 1},
    {"expert_table_1.txt", 1},       {"expert_table_2.txt", 0},
};

static int has_no_duplicate(Grid g) {
  for (int s = 0; s < SUBSET_COUNT; s++) {
    int seen[9] = {0};
    for (int k = 0; k < 9; k++) {
      int v = g->allSubsets.subsets[s][k]->value;
      if (v == 0)
        continue;
      if (seen[v - 1])
        return 0;
      seen[v - 1] = 1;
    }
  }
  return 1;
}

static void test_solve_all_tables(void) {
  char path[512];
  for (size_t i = 0; i < sizeof(k_tables) / sizeof(k_tables[0]); i++) {
    snprintf(path, sizeof(path), "../%s/%s", TABLES_DIR, k_tables[i].file);
    Grid g = load_sudoku_from_file(path);
    CU_ASSERT_PTR_NOT_NULL_FATAL(g);

    clock_t t0 = clock();
    char ok = solve(g);
    double dt = (double)(clock() - t0) / CLOCKS_PER_SEC;

    if (k_tables[i].must_solve) {
      CU_ASSERT_TRUE(ok);
      CU_ASSERT_EQUAL(grid_filled_count(g), 81);
    }
    CU_ASSERT_TRUE(has_no_duplicate(g));
    CU_ASSERT_TRUE(dt < PER_GRID_BUDGET_SEC);

    delete_grid(g);
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
