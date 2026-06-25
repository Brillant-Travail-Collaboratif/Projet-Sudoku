#include "cell_internal.h"

SolverStep solverHistory[GRID_CELL_COUNT];
int historyIndex;
int deductionCount;

SudokuCell *create_cell(const char value,
                        const char candidates[CANDIDATE_COUNT]) {
  if (is_value_invalid(value) || are_candidates_invalid(candidates))
    return NULL;

  SudokuCell *cell = malloc(sizeof(SudokuCell));
  if (cell == NULL)
    return NULL;

  cell->value = value;
  for (unsigned char i = 0; i < CANDIDATE_COUNT; i++) {
    cell->candidates[i] = candidates[i];
  }

  return cell;
}

void delete_cell(SudokuCell *cell) {
  if (cell != NULL)
    free(cell);
}

char is_value_invalid(char value) {
  if (value < 0 || value > CANDIDATE_COUNT) {
    return 1;
  }
  return 0;
}

char are_candidates_invalid(const char candidates[CANDIDATE_COUNT]) {
  if (candidates == NULL)
    return 1;

  for (unsigned char i = 0; i < CANDIDATE_COUNT; i++) {
    if (candidates[i] < 0 || candidates[i] > 1)
      return 1;
  }
  return 0;
}

char get_cell_value(SudokuCell *cell) {
  if (cell == NULL || is_value_invalid(cell->value))
    return 0;

  return cell->value;
}

char *get_cell_candidates(SudokuCell *cell) {
  if (cell == NULL || are_candidates_invalid(cell->candidates))
    return NULL;

  return cell->candidates;
}

char set_cell_value(SudokuCell *cell, char value, unsigned char isGuess) {
  if (cell == NULL || is_value_invalid(value))
    return 1;

  if (value != 0 && historyIndex >= GRID_CELL_COUNT)
    return 1;

  cell->value = value;

  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++)
    cell->candidates[d] = value == 0 ? 1 : 0;

  if (value != 0)
    cell->candidates[value - 1] = 1;

  if (value != 0) {
    solverHistory[historyIndex].cell = cell;
    solverHistory[historyIndex].value = value;
    solverHistory[historyIndex++].isGuess = isGuess;
    if (!isGuess)
      deductionCount++;
  }
  return 0;
}

char set_cell_candidates(SudokuCell *cell, char candidates[CANDIDATE_COUNT]) {
  if (cell == NULL || are_candidates_invalid(candidates))
    return 1;

  for (unsigned char i = 0; i < CANDIDATE_COUNT; i++) {
    cell->candidates[i] = candidates[i];
  }
  return 0;
}
