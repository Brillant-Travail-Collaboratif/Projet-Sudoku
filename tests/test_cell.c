#include "../src/grid/cell.h"
#include "../src/sudoku_types/type_history.h"
#include <CUnit/CUnit.h>

static void set_all_candidates(char candidates[CANDIDATE_COUNT]) {
  for (int i = 0; i < CANDIDATE_COUNT; i++)
    candidates[i] = 1;
}

static void test_create_and_get(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  CU_ASSERT_PTR_NOT_NULL_FATAL(cell);
  CU_ASSERT_EQUAL(get_cell_value(cell), 0);
  char *storedCandidates = get_cell_candidates(cell);
  CU_ASSERT_PTR_NOT_NULL_FATAL(storedCandidates);
  for (int i = 0; i < CANDIDATE_COUNT; i++)
    CU_ASSERT_EQUAL(storedCandidates[i], 1);
  delete_cell(cell);
}

static void test_create_rejects_invalid_input(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  candidates[3] = 2;

  CU_ASSERT_PTR_NULL(create_cell(10, candidates));
  CU_ASSERT_PTR_NULL(create_cell(0, candidates));
  CU_ASSERT_PTR_NULL(create_cell(0, NULL));
}

static void test_set_value_collapses_and_journals(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  CU_ASSERT_PTR_NOT_NULL_FATAL(cell);

  historyIndex = 0;
  CU_ASSERT_EQUAL(set_cell_value(cell, 5, 0), 0);
  CU_ASSERT_EQUAL(get_cell_value(cell), 5);

  char *storedCandidates = cell->candidates;
  CU_ASSERT_EQUAL(storedCandidates[4], 1);
  CU_ASSERT_EQUAL(storedCandidates[0], 0);
  /* A non-zero placement is journalled. */
  CU_ASSERT_EQUAL(historyIndex, 1);
  CU_ASSERT_EQUAL(solverHistory[0].value, 5);
  CU_ASSERT_EQUAL(solverHistory[0].isGuess, 0);
  delete_cell(cell);
}

static void test_set_zero_does_not_journal(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(5, candidates);
  historyIndex = 0;

  CU_ASSERT_EQUAL(set_cell_value(cell, 0, 0), 0);
  CU_ASSERT_EQUAL(historyIndex, 0);
  CU_ASSERT_EQUAL(get_cell_value(cell), 0);
  for (int i = 0; i < CANDIDATE_COUNT; i++)
    CU_ASSERT_EQUAL(cell->candidates[i], 1);
  delete_cell(cell);
}

static void test_deduction_counter(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  historyIndex = 0;
  deductionCount = 0;
  set_cell_value(cell, 3, 0); /* deduction */
  set_cell_value(cell, 4, 1); /* guess */
  CU_ASSERT_EQUAL(deductionCount, 1);
  delete_cell(cell);
}

static void test_invalid_value_rejected(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  CU_ASSERT_EQUAL(set_cell_value(cell, 10, 0), 1); /* out of 0..9 */
  CU_ASSERT_EQUAL(set_cell_value(NULL, 5, 0), 1);
  CU_ASSERT_EQUAL(get_cell_value(NULL), 0);
  cell->value = 10;
  CU_ASSERT_EQUAL(get_cell_value(cell), 0);
  cell->value = 10;
  CU_ASSERT_EQUAL(get_cell_value(cell), 0);
  delete_cell(cell);
  delete_cell(NULL);
}

static void test_set_and_get_candidates(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  CU_ASSERT_PTR_NOT_NULL_FATAL(cell);

  char newCandidates[CANDIDATE_COUNT] = {0};
  newCandidates[0] = 1;
  newCandidates[8] = 1;
  CU_ASSERT_EQUAL(set_cell_candidates(cell, newCandidates), 0);

  char *stored = get_cell_candidates(cell);
  CU_ASSERT_PTR_NOT_NULL_FATAL(stored);
  for (int i = 0; i < CANDIDATE_COUNT; i++)
    CU_ASSERT_EQUAL(stored[i], newCandidates[i]);

  newCandidates[4] = -1;
  CU_ASSERT_EQUAL(set_cell_candidates(cell, newCandidates), 1);
  CU_ASSERT_EQUAL(set_cell_candidates(NULL, candidates), 1);
  CU_ASSERT_EQUAL(set_cell_candidates(cell, NULL), 1);

  cell->candidates[0] = 2;
  CU_ASSERT_PTR_NULL(get_cell_candidates(cell));
  delete_cell(cell);
}

static void test_history_overflow_is_rejected(void) {
  char candidates[CANDIDATE_COUNT];
  set_all_candidates(candidates);
  SudokuCell *cell = create_cell(0, candidates);
  CU_ASSERT_PTR_NOT_NULL_FATAL(cell);

  historyIndex = GRID_CELL_COUNT;
  CU_ASSERT_EQUAL(set_cell_value(cell, 1, 0), 1);
  CU_ASSERT_EQUAL(cell->value, 0);
  for (int i = 0; i < CANDIDATE_COUNT; i++)
    CU_ASSERT_EQUAL(cell->candidates[i], 1);
  historyIndex = 0;
  delete_cell(cell);
}

int register_cell_tests(void) {
  CU_pSuite suite = CU_add_suite("cell", NULL, NULL);
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
      CU_add_test(suite, "set/get candidates", test_set_and_get_candidates) ==
      NULL ||
      CU_add_test(suite, "history overflow rejected",
                  test_history_overflow_is_rejected) == NULL)
    return 1;
  return 0;
}
