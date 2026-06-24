//
// Created by Nicolas Gouaux on 16/06/2026.
//
#include "../src/interface/cli.h"
#include "../src/interface/cli_internal.h"
#include <CUnit/CUnit.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

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

static void test_parse_generate_and_seed_flags(void) {
  CliOptions options;
  char *argv[] = {"sudoku", "-generate", "expert", "-seed", "42"};

  CU_ASSERT_TRUE(parse_options(5, argv, &options));
  CU_ASSERT_EQUAL(options.difficulty, EXPERT);
  CU_ASSERT_EQUAL(options.seed, 42u);
  CU_ASSERT_TRUE(options.has_difficulty);
  CU_ASSERT_TRUE(options.has_seed);
  CU_ASSERT_FALSE(options.benchmark);
  CU_ASSERT_PTR_NULL(options.load_file);
  CU_ASSERT_PTR_NULL(options.write_filepath);
  free_options(&options);
}

static void test_parse_write_flag(void) {
  CliOptions options;
  char *argv[] = {"sudoku", "-write", "/tmp/sudoku_cli_out.txt"};

  CU_ASSERT_TRUE(parse_options(3, argv, &options));
  CU_ASSERT_STRING_EQUAL(options.write_filepath, "/tmp/sudoku_cli_out.txt");
  CU_ASSERT_PTR_NULL(options.load_file);
  free_options(&options);
}

static void test_parse_load_allows_verbose_and_interactive(void) {
  CliOptions options;
  char *argv[] = {"sudoku", "-load", "trivial_table_1.txt", "-verbose",
                  "-interactive"};

  CU_ASSERT_TRUE(parse_options(5, argv, &options));
  CU_ASSERT_PTR_NOT_NULL_FATAL(options.load_file);
  CU_ASSERT_PTR_NOT_NULL(strstr(options.load_file, "trivial_table_1.txt"));
  CU_ASSERT_TRUE(options.verbose);
  CU_ASSERT_TRUE(options.interactive);
  free_options(&options);
}

static void test_parse_rejects_missing_flag_values(void) {
  CliOptions options;
  char *missing_generate[] = {"sudoku", "-generate"};
  char *bad_generate[] = {"sudoku", "-generate", "unknown"};
  char *missing_seed[] = {"sudoku", "-seed"};
  char *bad_seed[] = {"sudoku", "-seed", "abc"};
  char *missing_write[] = {"sudoku", "-write"};

  CU_ASSERT_FALSE(parse_options(2, missing_generate, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(3, bad_generate, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(2, missing_seed, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(3, bad_seed, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(2, missing_write, &options));
  free_options(&options);
}

static void test_parse_rejects_duplicate_files(void) {
  CliOptions options;
  char *duplicate_load[] = {"sudoku", "-load", "a.txt", "-load", "b.txt"};
  char *duplicate_write[] = {"sudoku", "-write", "a.txt", "-write", "b.txt"};

  CU_ASSERT_FALSE(parse_options(5, duplicate_load, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(5, duplicate_write, &options));
  free_options(&options);
}

static void test_parse_rejects_invalid_mode_combinations(void) {
  CliOptions options;
  char *benchmark_generate[] = {"sudoku", "-benchmark", "-generate", "basic"};
  char *benchmark_write[] = {"sudoku", "-benchmark", "-write", "out.txt"};
  char *load_generate[] = {"sudoku", "-load", "trivial_table_1.txt",
                           "-generate", "basic"};
  char *load_seed[] = {"sudoku", "-load", "trivial_table_1.txt", "-seed", "7"};

  CU_ASSERT_FALSE(parse_options(4, benchmark_generate, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(4, benchmark_write, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(5, load_generate, &options));
  free_options(&options);
  CU_ASSERT_FALSE(parse_options(5, load_seed, &options));
  free_options(&options);
}

static void test_parse_benchmark_allows_verbose(void) {
  CliOptions options;
  char *argv[] = {"sudoku", "-benchmark", "-verbose"};

  CU_ASSERT_TRUE(parse_options(3, argv, &options));
  CU_ASSERT_TRUE(options.benchmark);
  CU_ASSERT_TRUE(options.verbose);
  free_options(&options);
}

static void test_parse_rejects_seed_overflow(void) {
  CliOptions options;
  char seed_text[32];
  snprintf(seed_text, sizeof(seed_text), "%lu", (unsigned long)UINT_MAX + 1UL);
  char *argv[] = {"sudoku", "-seed", seed_text};

  CU_ASSERT_FALSE(parse_options(3, argv, &options));
  free_options(&options);
}

int register_cli_tests(void) {
  CU_pSuite suite = CU_add_suite("cli", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "help", test_cli_help_returns_success) == NULL ||
      CU_add_test(suite, "bad argument", test_cli_rejects_bad_argument) ==
      NULL ||
      CU_add_test(suite, "bad benchmark dir",
                  test_cli_rejects_bad_benchmark_dir) == NULL ||
      CU_add_test(suite, "parse generate and seed flags",
                  test_parse_generate_and_seed_flags) == NULL ||
      CU_add_test(suite, "parse write flag", test_parse_write_flag) == NULL ||
      CU_add_test(suite, "parse load allows verbose and interactive",
                  test_parse_load_allows_verbose_and_interactive) == NULL ||
      CU_add_test(suite, "parse rejects missing flag values",
                  test_parse_rejects_missing_flag_values) == NULL ||
      CU_add_test(suite, "parse rejects duplicate files",
                  test_parse_rejects_duplicate_files) == NULL ||
      CU_add_test(suite, "parse rejects invalid mode combinations",
                  test_parse_rejects_invalid_mode_combinations) == NULL ||
      CU_add_test(suite, "parse benchmark allows verbose",
                  test_parse_benchmark_allows_verbose) == NULL ||
      CU_add_test(suite, "parse rejects seed overflow",
                  test_parse_rejects_seed_overflow) == NULL)
    return 1;
  return 0;
}
