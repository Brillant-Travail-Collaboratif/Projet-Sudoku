#include "../src/grid/tile.h"
#include "../src/sudoku_types/type_history.h"
#include <CUnit/CUnit.h>

static void all_possible(char p[NUMBER_OF_POSSIBLE]) {
  for (int i = 0; i < NUMBER_OF_POSSIBLE; i++)
    p[i] = 1;
}

static void test_create_and_get(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);
  CU_ASSERT_EQUAL(get_tile_value(t), 0);
  char *q = get_tile_possibles(t);
  CU_ASSERT_PTR_NOT_NULL_FATAL(q);
  for (int i = 0; i < NUMBER_OF_POSSIBLE; i++)
    CU_ASSERT_EQUAL(q[i], 1);
  delete_tile(t);
}

static void test_create_rejects_invalid_input(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  p[3] = 2;

  CU_ASSERT_PTR_NULL(create_tile(10, p));
  CU_ASSERT_PTR_NULL(create_tile(0, p));
  CU_ASSERT_PTR_NULL(create_tile(0, NULL));
}

static void test_set_value_collapses_and_journals(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);

  history_index = 0;
  CU_ASSERT_EQUAL(set_tile_value(t, 5, 0), 0);
  CU_ASSERT_EQUAL(get_tile_value(t), 5);

  char *q = t->possible;
  CU_ASSERT_EQUAL(q[4], 1);
  CU_ASSERT_EQUAL(q[0], 0);
  /* A non-zero placement is journalled. */
  CU_ASSERT_EQUAL(history_index, 1);
  CU_ASSERT_EQUAL(history[0].value, 5);
  CU_ASSERT_EQUAL(history[0].supposed, 0);
  delete_tile(t);
}

static void test_set_zero_does_not_journal(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(5, p);
  history_index = 0;

  CU_ASSERT_EQUAL(set_tile_value(t, 0, 0), 0);
  CU_ASSERT_EQUAL(history_index, 0);
  CU_ASSERT_EQUAL(get_tile_value(t), 0);
  for (int i = 0; i < NUMBER_OF_POSSIBLE; i++)
    CU_ASSERT_EQUAL(t->possible[i], 1);
  delete_tile(t);
}

static void test_deduction_counter(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  history_index = 0;
  deduction_count = 0;
  set_tile_value(t, 3, 0); /* deduction */
  set_tile_value(t, 4, 1); /* supposition */
  CU_ASSERT_EQUAL(deduction_count, 1);
  delete_tile(t);
}

static void test_invalid_value_rejected(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  CU_ASSERT_EQUAL(set_tile_value(t, 10, 0), 1); /* out of 0..9 */
  CU_ASSERT_EQUAL(set_tile_value(NULL, 5, 0), 1);
  CU_ASSERT_EQUAL(get_tile_value(NULL), 0);
  t->value = 10;
  CU_ASSERT_EQUAL(get_tile_value(t), 0);
  t->value = 10;
  CU_ASSERT_EQUAL(get_tile_value(t), 0);
  delete_tile(t);
  delete_tile(NULL);
}

static void test_set_and_get_possibles(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);

  char next[NUMBER_OF_POSSIBLE] = {0};
  next[0] = 1;
  next[8] = 1;
  CU_ASSERT_EQUAL(set_tile_possibles(t, next), 0);

  char *stored = get_tile_possibles(t);
  CU_ASSERT_PTR_NOT_NULL_FATAL(stored);
  for (int i = 0; i < NUMBER_OF_POSSIBLE; i++)
    CU_ASSERT_EQUAL(stored[i], next[i]);

  next[4] = -1;
  CU_ASSERT_EQUAL(set_tile_possibles(t, next), 1);
  CU_ASSERT_EQUAL(set_tile_possibles(NULL, p), 1);
  CU_ASSERT_EQUAL(set_tile_possibles(t, NULL), 1);

  t->possible[0] = 2;
  CU_ASSERT_PTR_NULL(get_tile_possibles(t));
  delete_tile(t);
}

static void test_history_overflow_is_rejected(void) {
  char p[NUMBER_OF_POSSIBLE];
  all_possible(p);
  SudokuTile *t = create_tile(0, p);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);

  history_index = GRID_SIZE;
  CU_ASSERT_EQUAL(set_tile_value(t, 1, 0), 1);
  CU_ASSERT_EQUAL(t->value, 0);
  for (int i = 0; i < NUMBER_OF_POSSIBLE; i++)
    CU_ASSERT_EQUAL(t->possible[i], 1);
  history_index = 0;
  delete_tile(t);
}

int register_tile_tests(void) {
  CU_pSuite suite = CU_add_suite("tile", NULL, NULL);
  if (suite == NULL)
    return 1;
  if (CU_add_test(suite, "create/get", test_create_and_get) == NULL ||
      CU_add_test(suite, "set collapses + journals",
                  test_set_value_collapses_and_journals) == NULL ||
      CU_add_test(suite, "set 0 no journal", test_set_zero_does_not_journal) ==
      NULL ||
      CU_add_test(suite, "create rejects invalid input",
                  test_create_rejects_invalid_input) == NULL ||
      CU_add_test(suite, "deduction counter", test_deduction_counter) == NULL ||
      CU_add_test(suite, "invalid rejected", test_invalid_value_rejected) ==
      NULL ||
      CU_add_test(suite, "set/get possibles", test_set_and_get_possibles) ==
      NULL ||
      CU_add_test(suite, "history overflow rejected",
                  test_history_overflow_is_rejected) == NULL)
    return 1;
  return 0;
}
