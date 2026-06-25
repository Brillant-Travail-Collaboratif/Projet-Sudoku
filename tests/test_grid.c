#include "../src/grid/grid.h"
#include <CUnit/CUnit.h>

static void test_create_grid_starts_empty(void) {
    Grid grid = create_grid();
    CU_ASSERT_PTR_NOT_NULL_FATAL(grid);
    CU_ASSERT_EQUAL(count_filled_cells(grid), 0);

    for (unsigned char y = 1; y <= 9; y++)
        for (unsigned char x = 1; x <= 9; x++) {
            CU_ASSERT_EQUAL(get_grid_value_xy(grid, x, y), 0);
            char *cellCandidates = get_grid_candidates_xy(grid, x, y);
            CU_ASSERT_PTR_NOT_NULL_FATAL(cellCandidates);
            for (int d = 0; d < 9; d++)
                CU_ASSERT_EQUAL(cellCandidates[d], 1);
        }
    delete_grid(grid);
}

static void test_set_get_value(void) {
    Grid grid = create_grid();
    CU_ASSERT_EQUAL(set_grid_value_xy(grid, 3, 4, 7, 0), 0);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 3, 4), 7);
    CU_ASSERT_EQUAL(count_filled_cells(grid), 1);

    char *cellCandidates = get_grid_candidates_xy(grid, 3, 4);
    CU_ASSERT_PTR_NOT_NULL_FATAL(cellCandidates);
    CU_ASSERT_EQUAL(cellCandidates[6], 1);
    CU_ASSERT_EQUAL(cellCandidates[0], 0);
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
    CU_ASSERT_EQUAL(count_filled_cells(grid), 0);
    delete_grid(grid);
}

static void test_set_raw_does_not_journal(void) {
    Grid grid = create_grid();
    CU_ASSERT_EQUAL(set_grid_value_raw(grid, 1, 1, 4), 0);
    CU_ASSERT_EQUAL(get_grid_value_xy(grid, 1, 1), 4);
    char *cellCandidates = get_grid_candidates_xy(grid, 1, 1);
    CU_ASSERT_EQUAL(cellCandidates[3], 1);
    CU_ASSERT_EQUAL(cellCandidates[0], 0);
    delete_grid(grid);
}

static void test_set_raw_rejects_invalid_input(void) {
    Grid grid = create_grid();
    CU_ASSERT_EQUAL(set_grid_value_raw(NULL, 1, 1, 4), 1);
    CU_ASSERT_EQUAL(set_grid_value_raw(grid, 0, 1, 4), 1);
    CU_ASSERT_EQUAL(set_grid_value_raw(grid, 1, 10, 4), 1);
    CU_ASSERT_EQUAL(set_grid_value_raw(grid, 1, 1, 10), 1);
    CU_ASSERT_EQUAL(set_grid_value_raw(grid, 1, 1, -1), 1);
    CU_ASSERT_EQUAL(count_filled_cells(grid), 0);
    delete_grid(grid);
}

static void test_reset_candidates_rebuilds_from_values(void) {
    Grid grid = create_grid();
    set_grid_value_raw(grid, 1, 1, 5);
    char *filledCandidates = get_grid_candidates_xy(grid, 1, 1);
    char *emptyCandidates = get_grid_candidates_xy(grid, 2, 1);
    for (int d = 0; d < CANDIDATE_COUNT; d++) {
        filledCandidates[d] = 1;
        emptyCandidates[d] = 0;
    }

    reset_grid_candidates(grid);

    for (int d = 0; d < CANDIDATE_COUNT; d++) {
        CU_ASSERT_EQUAL(filledCandidates[d], d == 4 ? 1 : 0);
        CU_ASSERT_EQUAL(emptyCandidates[d], 1);
    }
    reset_grid_candidates(NULL);
    delete_grid(grid);
}

static void test_set_grid_candidates(void) {
    Grid grid = create_grid();
    char candidates[CANDIDATE_COUNT] = {0};
    candidates[1] = 1;
    candidates[7] = 1;

    CU_ASSERT_EQUAL(set_grid_candidates_xy(grid, 2, 2, candidates), 0);
    char *stored = get_grid_candidates_xy(grid, 2, 2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(stored);
    for (int d = 0; d < CANDIDATE_COUNT; d++)
        CU_ASSERT_EQUAL(stored[d], candidates[d]);

    candidates[0] = 2;
    CU_ASSERT_EQUAL(set_grid_candidates_xy(grid, 2, 2, candidates), 1);
    CU_ASSERT_EQUAL(set_grid_candidates_xy(grid, 0, 2, candidates), 1);
    CU_ASSERT_EQUAL(set_grid_candidates_xy(NULL, 2, 2, candidates), 1);
    CU_ASSERT_EQUAL(set_grid_candidates_xy(grid, 2, 2, NULL), 1);
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
    CU_ASSERT_EQUAL(count_filled_cells(NULL), 0);
    CU_ASSERT_EQUAL(get_grid_value_xy(NULL, 1, 1), 0);
    CU_ASSERT_PTR_NULL(get_grid_candidates_xy(NULL, 1, 1));
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
        CU_add_test(suite, "raw set rejects invalid input",
                    test_set_raw_rejects_invalid_input) == NULL ||
        CU_add_test(suite, "reset candidates", test_reset_candidates_rebuilds_from_values) == NULL ||
        CU_add_test(suite, "set candidates", test_set_grid_candidates) == NULL ||
        CU_add_test(suite, "clone independent", test_clone_is_independent) == NULL ||
        CU_add_test(suite, "NULL safety", test_null_safety) == NULL)
        return 1;
    return 0;
}
