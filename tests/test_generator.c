#include "../src/generator/difficulty.h"
#include "../src/generator/generator.h"
#include "../src/generator/random.h"
#include "../src/grid/grid.h"
#include "../src/solver/solver.h"
#include "../src/sudoku_types/type_difficulty.h"
#include "../src/sudoku_types/type_random.h"
#include <CUnit/CUnit.h>
#include <limits.h>

static void test_random_is_deterministic(void) {
  RandomGenerator firstGenerator = {12345u};
  RandomGenerator secondGenerator = {12345u};
  for (int i = 0; i < 50; i++) {
    int firstValue = generate_random_int(&firstGenerator, 100);
    int secondValue = generate_random_int(&secondGenerator, 100);
    CU_ASSERT_EQUAL(firstValue, secondValue);
    CU_ASSERT_TRUE(firstValue >= 0 && firstValue < 100);
  }
}

static void test_random_limit_one(void) {
  RandomGenerator generator = {7u};
  CU_ASSERT_EQUAL(generate_random_int(&generator, 1), 0);
  CU_ASSERT_EQUAL(generate_random_int(&generator, 0), 0);
  CU_ASSERT_EQUAL(generate_random_int(&generator, -3), 0);
}

static void test_shuffle_is_a_permutation(void) {
  RandomGenerator generator = {99u};
  int values[9];
  for (int i = 0; i < 9; i++)
    values[i] = i;
  shuffle_integers(&generator, values, 9);
  int seen[9] = {0};
  for (int i = 0; i < 9; i++) {
    CU_ASSERT_TRUE(values[i] >= 0 && values[i] < 9);
    seen[values[i]]++;
  }
  for (int i = 0; i < 9; i++)
    CU_ASSERT_EQUAL(seen[i], 1);

  shuffle_integers(&generator, values, 0);
  shuffle_integers(&generator, values, 1);
  CU_ASSERT_TRUE(values[0] >= 0 && values[0] < 9);
}

static void test_parse_difficulty(void) {
  Difficulty difficulty;
  CU_ASSERT_EQUAL(parse_difficulty("trivial", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, TRIVIAL);
  CU_ASSERT_EQUAL(parse_difficulty("basic", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, BASIC);
  CU_ASSERT_EQUAL(parse_difficulty("intermediate", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, INTERMEDIATE);
  CU_ASSERT_EQUAL(parse_difficulty("difficile", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, DIFFICULT);
  CU_ASSERT_EQUAL(parse_difficulty("difficult", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, DIFFICULT);
  CU_ASSERT_EQUAL(parse_difficulty("expert", &difficulty), 1);
  CU_ASSERT_EQUAL(difficulty, EXPERT);
  CU_ASSERT_EQUAL(parse_difficulty("nope", &difficulty), 0);
  CU_ASSERT_EQUAL(parse_difficulty(NULL, &difficulty), 0);
  CU_ASSERT_EQUAL(parse_difficulty("basic", NULL), 0);
}

static void test_get_difficulty_name(void) {
  CU_ASSERT_STRING_EQUAL(get_difficulty_name(TRIVIAL), "trivial");
  CU_ASSERT_STRING_EQUAL(get_difficulty_name(BASIC), "basic");
  CU_ASSERT_STRING_EQUAL(get_difficulty_name(INTERMEDIATE), "intermediate");
  CU_ASSERT_STRING_EQUAL(get_difficulty_name(DIFFICULT), "difficile");
  CU_ASSERT_STRING_EQUAL(get_difficulty_name(EXPERT), "expert");
  CU_ASSERT_STRING_EQUAL(get_difficulty_name((Difficulty)99), "basic");
}

static void test_get_difficulty_target(void) {
  DifficultyTarget target = get_difficulty_target(TRIVIAL);
  CU_ASSERT_EQUAL(target.minRemoved, 1);
  CU_ASSERT_EQUAL(target.maxRemoved, 40);
  CU_ASSERT_EQUAL(target.minGuesses, 0);
  CU_ASSERT_EQUAL(target.maxGuesses, 0);

  target = get_difficulty_target(BASIC);
  CU_ASSERT_EQUAL(target.minRemoved, 41);
  CU_ASSERT_EQUAL(target.maxRemoved, 50);
  CU_ASSERT_EQUAL(target.minGuesses, 0);
  CU_ASSERT_EQUAL(target.maxGuesses, 0);

  target = get_difficulty_target(INTERMEDIATE);
  CU_ASSERT_EQUAL(target.minRemoved, 51);
  CU_ASSERT_EQUAL(target.maxRemoved, 64);
  CU_ASSERT_EQUAL(target.minGuesses, 0);
  CU_ASSERT_EQUAL(target.maxGuesses, 0);

  target = get_difficulty_target(DIFFICULT);
  CU_ASSERT_EQUAL(target.minRemoved, 1);
  CU_ASSERT_EQUAL(target.maxRemoved, 70);
  CU_ASSERT_EQUAL(target.minGuesses, 1);
  CU_ASSERT_EQUAL(target.maxGuesses, 5);

  target = get_difficulty_target(EXPERT);
  CU_ASSERT_EQUAL(target.minRemoved, 1);
  CU_ASSERT_EQUAL(target.maxRemoved, 70);
  CU_ASSERT_EQUAL(target.minGuesses, 6);
  CU_ASSERT_EQUAL(target.maxGuesses, INT_MAX);

  target = get_difficulty_target((Difficulty)99);
  CU_ASSERT_EQUAL(target.minRemoved, 41);
  CU_ASSERT_EQUAL(target.maxRemoved, 50);
}

static void test_generate_is_valid_and_reproducible(void) {
  Grid firstGrid = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(firstGrid);

  int filledCount = count_filled_cells(firstGrid);
  CU_ASSERT_TRUE(filledCount > 0 && filledCount < 81);

  Grid gridToSolve = clone_grid_values(firstGrid);
  CU_ASSERT_EQUAL(solve(gridToSolve), 1);
  delete_grid(gridToSolve);

  Grid secondGrid = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(secondGrid);
  for (int i = 0; i < 81; i++)
    CU_ASSERT_EQUAL(firstGrid->cells[i].value, secondGrid->cells[i].value);

  CU_ASSERT_EQUAL(count_removed_cells(firstGrid), 81 - filledCount);

  delete_grid(secondGrid);
  delete_grid(firstGrid);
}

static void test_matches_difficulty_and_removed_count(void) {
  Grid completed = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(completed);
  for (int i = 0; i < 81; i++)
    set_grid_value_raw(completed, (i % 9) + 1, (i / 9) + 1,
                       (char)((i % 9) + 1));

  CU_ASSERT_EQUAL(count_removed_cells(completed), 0);
  CU_ASSERT_EQUAL(count_removed_cells(NULL), 81);
  CU_ASSERT_EQUAL(matches_difficulty(NULL, BASIC), 0);
  CU_ASSERT_EQUAL(matches_difficulty(completed, BASIC), 0);
  delete_grid(completed);

  Grid basicGrid = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(basicGrid);
  CU_ASSERT_EQUAL(matches_difficulty(basicGrid, BASIC), 1);
  delete_grid(basicGrid);
}

int register_generator_tests(void) {
  CU_pSuite suite = CU_add_suite("generator", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "random deterministic",
                  test_random_is_deterministic) == NULL ||
      CU_add_test(suite, "random limit<=1", test_random_limit_one) == NULL ||
      CU_add_test(suite, "shuffle permutation",
                  test_shuffle_is_a_permutation) == NULL ||
      CU_add_test(suite, "parse difficulty", test_parse_difficulty) == NULL ||
      CU_add_test(suite, "difficulty name", test_get_difficulty_name) ==
          NULL ||
      CU_add_test(suite, "difficulty target", test_get_difficulty_target) ==
          NULL ||
      CU_add_test(suite, "generate valid+reproducible",
                  test_generate_is_valid_and_reproducible) == NULL ||
      CU_add_test(suite, "matches difficulty and removed count",
                  test_matches_difficulty_and_removed_count) == NULL)
    return 1;
  return 0;
}
