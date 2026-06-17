//
// Created by Meike Rayan on 16/06/2026.
//
#include "../src/cli/cli.h"
#include <CUnit/CUnit.h>

static void test_cli_help_returns_success(void) {
    char *argv[] = {"sudoku", "-h"};
    CU_ASSERT_EQUAL(cli_run(2, argv), 0);
}

static void test_cli_rejects_bad_argument(void) {
    char *argv[] = {"sudoku", "-not-an-option"};
    CU_ASSERT_EQUAL(cli_run(2, argv), 1);
}

static void test_cli_rejects_bad_benchmark_dir(void) {
    char *argv[] = {"sudoku", "-benchmark", "/tmp/sudoku_missing_benchmark_dir"};
    CU_ASSERT_EQUAL(cli_run(3, argv), 1);
}

int register_cli_tests(void) {
    CU_pSuite suite = CU_add_suite("cli", NULL, NULL);
    if (suite == NULL)
        return 1;
    if (CU_add_test(suite, "help", test_cli_help_returns_success) == NULL ||
        CU_add_test(suite, "bad argument", test_cli_rejects_bad_argument) ==
        NULL ||
        CU_add_test(suite, "bad benchmark dir",
                    test_cli_rejects_bad_benchmark_dir) == NULL)
        return 1;
    return 0;
}
