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
  GeneratorRandom a = {12345u};
  GeneratorRandom b = {12345u};
  for (int i = 0; i < 50; i++) {
    int x = generator_random_int(&a, 100);
    int y = generator_random_int(&b, 100);
    CU_ASSERT_EQUAL(x, y);
    CU_ASSERT_TRUE(x >= 0 && x < 100);
  }
}

static void test_random_limit_one(void) {
  GeneratorRandom r = {7u};
  CU_ASSERT_EQUAL(generator_random_int(&r, 1), 0);
  CU_ASSERT_EQUAL(generator_random_int(&r, 0), 0);
  CU_ASSERT_EQUAL(generator_random_int(&r, -3), 0);
}

static void test_shuffle_is_a_permutation(void) {
  GeneratorRandom r = {99u};
  int v[9];
  for (int i = 0; i < 9; i++)
    v[i] = i;
  shuffle_ints(&r, v, 9);
  int seen[9] = {0};
  for (int i = 0; i < 9; i++) {
    CU_ASSERT_TRUE(v[i] >= 0 && v[i] < 9);
    seen[v[i]]++;
  }
  for (int i = 0; i < 9; i++)
    CU_ASSERT_EQUAL(seen[i], 1);

  shuffle_ints(&r, v, 0);
  shuffle_ints(&r, v, 1);
  CU_ASSERT_TRUE(v[0] >= 0 && v[0] < 9);
}

static void test_parse_difficulty(void) {
  Difficulty d;
  CU_ASSERT_EQUAL(parse_difficulty("trivial", &d), 1);
  CU_ASSERT_EQUAL(d, TRIVIAL);
  CU_ASSERT_EQUAL(parse_difficulty("basic", &d), 1);
  CU_ASSERT_EQUAL(d, BASIC);
  CU_ASSERT_EQUAL(parse_difficulty("intermediate", &d), 1);
  CU_ASSERT_EQUAL(d, INTERMEDIATE);
  CU_ASSERT_EQUAL(parse_difficulty("difficile", &d), 1);
  CU_ASSERT_EQUAL(d, DIFFICILE);
  CU_ASSERT_EQUAL(parse_difficulty("difficult", &d), 1);
  CU_ASSERT_EQUAL(d, DIFFICILE);
  CU_ASSERT_EQUAL(parse_difficulty("expert", &d), 1);
  CU_ASSERT_EQUAL(d, EXPERT);
  CU_ASSERT_EQUAL(parse_difficulty("nope", &d), 0);
  CU_ASSERT_EQUAL(parse_difficulty(NULL, &d), 0);
  CU_ASSERT_EQUAL(parse_difficulty("basic", NULL), 0);
}

static void test_difficulty_to_string(void) {
  CU_ASSERT_STRING_EQUAL(difficulty_to_string(TRIVIAL), "trivial");
  CU_ASSERT_STRING_EQUAL(difficulty_to_string(BASIC), "basic");
  CU_ASSERT_STRING_EQUAL(difficulty_to_string(INTERMEDIATE), "intermediate");
  CU_ASSERT_STRING_EQUAL(difficulty_to_string(DIFFICILE), "difficile");
  CU_ASSERT_STRING_EQUAL(difficulty_to_string(EXPERT), "expert");
  CU_ASSERT_STRING_EQUAL(difficulty_to_string((Difficulty)99), "basic");
}

static void test_target_for_difficulty(void) {
  DifficultyTarget t = target_for_difficulty(TRIVIAL);
  CU_ASSERT_EQUAL(t.min_removed, 1);
  CU_ASSERT_EQUAL(t.max_removed, 40);
  CU_ASSERT_EQUAL(t.min_suppositions, 0);
  CU_ASSERT_EQUAL(t.max_suppositions, 0);

  t = target_for_difficulty(BASIC);
  CU_ASSERT_EQUAL(t.min_removed, 41);
  CU_ASSERT_EQUAL(t.max_removed, 50);
  CU_ASSERT_EQUAL(t.min_suppositions, 0);
  CU_ASSERT_EQUAL(t.max_suppositions, 0);

  t = target_for_difficulty(INTERMEDIATE);
  CU_ASSERT_EQUAL(t.min_removed, 51);
  CU_ASSERT_EQUAL(t.max_removed, 64);
  CU_ASSERT_EQUAL(t.min_suppositions, 0);
  CU_ASSERT_EQUAL(t.max_suppositions, 0);

  t = target_for_difficulty(DIFFICILE);
  CU_ASSERT_EQUAL(t.min_removed, 1);
  CU_ASSERT_EQUAL(t.max_removed, 70);
  CU_ASSERT_EQUAL(t.min_suppositions, 1);
  CU_ASSERT_EQUAL(t.max_suppositions, 5);

  t = target_for_difficulty(EXPERT);
  CU_ASSERT_EQUAL(t.min_removed, 1);
  CU_ASSERT_EQUAL(t.max_removed, 70);
  CU_ASSERT_EQUAL(t.min_suppositions, 6);
  CU_ASSERT_EQUAL(t.max_suppositions, INT_MAX);

  t = target_for_difficulty((Difficulty)99);
  CU_ASSERT_EQUAL(t.min_removed, 41);
  CU_ASSERT_EQUAL(t.max_removed, 50);
}

static void test_generate_is_valid_and_reproducible(void) {
  Grid a = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(a);

  int filled = grid_filled_count(a);
  CU_ASSERT_TRUE(filled > 0 && filled < 81);

  Grid solving = clone_grid_values(a);
  CU_ASSERT_EQUAL(solve(solving), 1);
  delete_grid(solving);

  Grid b = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(b);
  for (int i = 0; i < 81; i++)
    CU_ASSERT_EQUAL(a->allTiles[i].value, b->allTiles[i].value);

  CU_ASSERT_EQUAL(removed_count(a), 81 - filled);

  delete_grid(b);
  delete_grid(a);
}

static void test_matches_difficulty_and_removed_count(void) {
  Grid completed = create_grid();
  CU_ASSERT_PTR_NOT_NULL_FATAL(completed);
  for (int i = 0; i < 81; i++)
    set_grid_value_raw(completed, (i % 9) + 1, (i / 9) + 1,
                       (char)((i % 9) + 1));

  CU_ASSERT_EQUAL(removed_count(completed), 0);
  CU_ASSERT_EQUAL(removed_count(NULL), 81);
  CU_ASSERT_EQUAL(matches_difficulty(NULL, BASIC), 0);
  CU_ASSERT_EQUAL(matches_difficulty(completed, BASIC), 0);
  delete_grid(completed);

  Grid basic = generate_sudoku(BASIC, 1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(basic);
  CU_ASSERT_EQUAL(matches_difficulty(basic, BASIC), 1);
  delete_grid(basic);
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
      CU_add_test(suite, "difficulty to string", test_difficulty_to_string) ==
          NULL ||
      CU_add_test(suite, "target for difficulty", test_target_for_difficulty) ==
          NULL ||
      CU_add_test(suite, "generate valid+reproducible",
                  test_generate_is_valid_and_reproducible) == NULL ||
      CU_add_test(suite, "matches difficulty and removed count",
                  test_matches_difficulty_and_removed_count) == NULL)
    return 1;
  return 0;
}
