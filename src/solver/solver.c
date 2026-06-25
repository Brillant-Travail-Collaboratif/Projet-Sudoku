#include "solver_internal.h"
#include <stddef.h>

unsigned char count_candidates(const SudokuCell *cell, char *candidate) {
  unsigned char count = 0;
  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++) {
    if (cell->candidates[d]) {
      count++;
      *candidate = (char)(d + 1);
    }
  }
  return count;
}

char solve_naked_singles(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < GRID_CELL_COUNT; i++) {
    /* Leave cells with assigned values unchanged. */
    if (grid->cells[i].value != 0)
      continue;

    char candidate = 0;
    const unsigned char count =
        count_candidates(&grid->cells[i], &candidate);
    if (count == 1) {
      set_cell_value(&grid->cells[i], candidate, 0);
      modified = 1;
    }
  }
  return modified;
}

char solve_hidden_singles_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char modified = 0;
  for (char value = 1; value <= CELLS_PER_UNIT; value++) {
    char isAlreadyPlaced = 0;
    unsigned char count = 0;
    unsigned char targetIndex = 0;

    for (unsigned char i = 0; i < CELLS_PER_UNIT; i++) {
      if (subset[i]->value == value) {
        isAlreadyPlaced = 1;
        break;
      }
      if (subset[i]->value == 0 && subset[i]->candidates[value - 1]) {
        count++;
        targetIndex = i;
      }
    }
    if (!isAlreadyPlaced && count == 1) {
      set_cell_value(subset[targetIndex], value, 0);
      modified = 1;
    }
  }
  return modified;
}

char solve_hidden_singles(Grid grid) {
  if (grid == NULL)
    return 0;

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (solve_hidden_singles_in_subset(grid->subsets.items[i]) != 0)
      modified = 1;
  }

  return modified;
}

char remove_cell_candidate(SudokuCell *cell, char value) {
  if (cell->candidates[value - 1]) {
    cell->candidates[value - 1] = 0;
    return 1;
  }
  return 0;
}

char clean_row(SudokuCell *row) {
  if (row == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < CELLS_PER_UNIT; i++) {
    const char value = row[i].value;
    if (value == 0)
      continue;

    /* Remove the assigned value from every other unsolved cell. */
    for (unsigned char j = 0; j < CELLS_PER_UNIT; j++) {
      if (j == i)
        continue;
      if (row[j].value != 0)
        continue;
      if (remove_cell_candidate(&row[j], value))
        modified = 1;
    }
  }
  return modified;
}

char clean_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < CELLS_PER_UNIT; i++) {
    const char value = subset[i]->value;
    if (value == 0)
      continue;

    for (unsigned char j = 0; j < CELLS_PER_UNIT; j++) {
      if (j == i)
        continue;
      if (subset[j]->value != 0)
        continue;
      if (remove_cell_candidate(subset[j], value))
        modified = 1;
    }
  }
  return modified;
}
char solve_hidden_singles_in_row(SudokuCell *row) {
  if (row == NULL)
    return 0;

  unsigned char modified = 0;
  for (unsigned char i = 0; i < CANDIDATE_COUNT; i++) {
    unsigned char candidateIndex = 0;
    for (unsigned char j = 0; j < CANDIDATE_COUNT; j++) {
      char *candidates = get_cell_candidates(&row[j]);
      if (candidates == NULL)
        continue;
      char candidateFlag = candidates[i];
      if (candidateFlag != 0 && candidateIndex != 0)
        candidateIndex = j;
      else if (candidateFlag != 0) {
        candidateIndex = 0;
        break;
      }
    }
    if (candidateIndex != 0) {
      set_cell_value(&row[candidateIndex], i + 1, 0);
      modified = 1;
    }
  }
  return modified;
}

char clean_grid(Grid grid) {
  if (grid == NULL)
    return 0;

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (clean_subset(grid->subsets.items[i]) != 0)
      modified = 1;
  }

  return modified;
}

char apply_rule_on_grid(Grid grid, char (*rule)(Subset)) {
  if (grid == NULL || rule == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (rule(grid->subsets.items[i]))
      modified = 1;
  }

  return modified;
}

unsigned char list_candidates(SudokuCell *cell,
                              char output[CANDIDATE_COUNT]) {
  unsigned char count = 0;
  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++) {
    if (cell->candidates[d])
      output[count++] = (char)(d + 1);
  }
  return count;
}

char clean_naked_pair_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < CELLS_PER_UNIT; i++) {
    if (subset[i]->value != 0)
      continue;
    char firstCandidates[CANDIDATE_COUNT];
    if (list_candidates(subset[i], firstCandidates) != 2)
      continue;

    for (unsigned char j = i + 1; j < CELLS_PER_UNIT; j++) {
      if (subset[j]->value != 0)
        continue;
      char secondCandidates[CANDIDATE_COUNT];
      if (list_candidates(subset[j], secondCandidates) != 2)
        continue;
      if (firstCandidates[0] != secondCandidates[0] ||
          firstCandidates[1] != secondCandidates[1])
        continue;

      /* Quand on trouve une paire , on nettoie les autres cases inconnues du
       * subset. */
      for (unsigned char k = 0; k < CELLS_PER_UNIT; k++) {
        if (k == i || k == j || subset[k]->value != 0)
          continue;
        if (remove_cell_candidate(subset[k], firstCandidates[0]))
          modified = 1;
        if (remove_cell_candidate(subset[k], firstCandidates[1]))
          modified = 1;
      }
    }
  }
  return modified;
}

char clean_naked_pairs(Grid grid) {
  return apply_rule_on_grid(grid, clean_naked_pair_in_subset);
}

char clean_hidden_pairs_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char candidates[CANDIDATE_COUNT][3];

  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++) {
    candidates[d][0] = 0;
    candidates[d][1] = 0;
    candidates[d][2] = 0;
  }

  for (unsigned char digit = 0; digit < CANDIDATE_COUNT; digit++) {
    for (unsigned char pos = 0; pos < CANDIDATE_COUNT; pos++) {
      if (subset[pos]->candidates[digit] != 0) {
        if (candidates[digit][2] < 2) {
          candidates[digit][candidates[digit][2]] = pos;
        }
        candidates[digit][2]++;
      }
    }
  }

  unsigned char modified = 0;

  for (unsigned char d1 = 0; d1 < CANDIDATE_COUNT; d1++) {
    if (candidates[d1][2] != 2)
      continue;

    for (unsigned char d2 = d1 + 1; d2 < CANDIDATE_COUNT; d2++) {
      if (candidates[d2][2] != 2)
        continue;

      unsigned char a1 = candidates[d1][0];
      unsigned char a2 = candidates[d1][1];
      unsigned char b1 = candidates[d2][0];
      unsigned char b2 = candidates[d2][1];

      if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) {
        for (unsigned char digit = 0; digit < CANDIDATE_COUNT; digit++) {
          if (digit != d1 && digit != d2) {
            if (subset[a1]->candidates[digit] == 1 ||
                subset[a2]->candidates[digit] == 1)
              modified = 1;
            subset[a1]->candidates[digit] = 0;
            subset[a2]->candidates[digit] = 0;
          }
        }
      }
    }
  }

  return modified;
}

char clean_hidden_pairs(Grid grid) {
  if (grid == NULL)
    return 0;

  return apply_rule_on_grid(grid, clean_hidden_pairs_in_subset);
}

char clean_naked_triple_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < CELLS_PER_UNIT; i++) {
    if (subset[i]->value != 0)
      continue;
    char firstCandidates[CANDIDATE_COUNT];
    int firstCount = list_candidates(subset[i], firstCandidates);
    if (firstCount < 2 || firstCount > 3)
      continue;

    for (unsigned char j = i + 1; j < CELLS_PER_UNIT; j++) {
      if (subset[j]->value != 0)
        continue;
      char secondCandidates[CANDIDATE_COUNT];
      int secondCount = list_candidates(subset[j], secondCandidates);
      if (secondCount < 2 || secondCount > 3)
        continue;

      for (unsigned char k = j + 1; k < CELLS_PER_UNIT; k++) {
        if (subset[k]->value != 0)
          continue;
        char thirdCandidates[CANDIDATE_COUNT];
        int thirdCount = list_candidates(subset[k], thirdCandidates);
        if (thirdCount < 2 || thirdCount > 3)
          continue;

        char unionValues[CANDIDATE_COUNT];
        int unionSize = 0;

        for (int a = 0; a < firstCount; a++) {
          char found = 0;
          for (int b = 0; b < unionSize; b++)
            if (unionValues[b] == firstCandidates[a]) {
              found = 1;
              break;
            }
          if (!found)
            unionValues[unionSize++] = firstCandidates[a];
        }

        for (int a = 0; a < secondCount; a++) {
          char found = 0;
          for (int b = 0; b < unionSize; b++)
            if (unionValues[b] == secondCandidates[a]) {
              found = 1;
              break;
            }
          if (!found)
            unionValues[unionSize++] = secondCandidates[a];
        }

        for (int a = 0; a < thirdCount; a++) {
          char found = 0;
          for (int b = 0; b < unionSize; b++)
            if (unionValues[b] == thirdCandidates[a]) {
              found = 1;
              break;
            }
          if (!found)
            unionValues[unionSize++] = thirdCandidates[a];
        }

        if (unionSize != 3)
          continue;

        for (unsigned char l = 0; l < CELLS_PER_UNIT; l++) {
          if (l == i || l == j || l == k || subset[l]->value != 0)
            continue;
          for (int a = 0; a < unionSize; a++)
            if (remove_cell_candidate(subset[l], unionValues[a]))
              modified = 1;
        }
      }
    }
  }
  return modified;
}

char clean_naked_triples(Grid grid) {
  return apply_rule_on_grid(grid, clean_naked_triple_in_subset);
}

char clean_hidden_triples_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char candidates[CANDIDATE_COUNT][4];

  for (unsigned char d = 0; d < CANDIDATE_COUNT; d++) {
    candidates[d][0] = 0;
    candidates[d][1] = 0;
    candidates[d][2] = 0;
    candidates[d][3] = 0;
  }

  for (unsigned char digit = 0; digit < CANDIDATE_COUNT; digit++) {
    for (unsigned char pos = 0; pos < CELLS_PER_UNIT; pos++) {
      if (subset[pos]->candidates[digit] != 0) {
        if (candidates[digit][3] < 3)
          candidates[digit][candidates[digit][3]] = pos;
        candidates[digit][3]++;
      }
    }
  }

  char modified = 0;

  for (unsigned char d1 = 0; d1 < CANDIDATE_COUNT; d1++) {
    if (candidates[d1][3] < 2 || candidates[d1][3] > 3)
      continue;

    for (unsigned char d2 = d1 + 1; d2 < CANDIDATE_COUNT; d2++) {
      if (candidates[d2][3] < 2 || candidates[d2][3] > 3)
        continue;

      for (unsigned char d3 = d2 + 1; d3 < CANDIDATE_COUNT; d3++) {
        if (candidates[d3][3] < 2 || candidates[d3][3] > 3)
          continue;

        unsigned char unionPositions[3];
        unsigned char unionSize = 0;

        for (unsigned char a = 0; a < candidates[d1][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d1][a]) {
              found = 1;
              break;
            }
          if (!found && unionSize < 3)
            unionPositions[unionSize++] = candidates[d1][a];
        }

        for (unsigned char a = 0; a < candidates[d2][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d2][a]) {
              found = 1;
              break;
            }
          if (!found && unionSize < 3)
            unionPositions[unionSize++] = candidates[d2][a];
        }

        for (unsigned char a = 0; a < candidates[d3][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d3][a]) {
              found = 1;
              break;
            }
          if (!found && unionSize < 3)
            unionPositions[unionSize++] = candidates[d3][a];
        }

        if (unionSize != 3)
          continue;

        unsigned char overflow = 0;
        if (candidates[d1][3] > 3 || candidates[d2][3] > 3 ||
            candidates[d3][3] > 3)
          overflow = 1;

        for (unsigned char a = 0; a < candidates[d1][3] && !overflow; a++) {
          unsigned char inUnion = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d1][a]) {
              inUnion = 1;
              break;
            }
          if (!inUnion)
            overflow = 1;
        }
        for (unsigned char a = 0; a < candidates[d2][3] && !overflow; a++) {
          unsigned char inUnion = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d2][a]) {
              inUnion = 1;
              break;
            }
          if (!inUnion)
            overflow = 1;
        }
        for (unsigned char a = 0; a < candidates[d3][3] && !overflow; a++) {
          unsigned char inUnion = 0;
          for (unsigned char b = 0; b < unionSize; b++)
            if (unionPositions[b] == candidates[d3][a]) {
              inUnion = 1;
              break;
            }
          if (!inUnion)
            overflow = 1;
        }
        if (overflow)
          continue;

        for (unsigned char p = 0; p < unionSize; p++) {
          unsigned char pos = unionPositions[p];
          for (unsigned char digit = 0; digit < CANDIDATE_COUNT; digit++) {
            if (digit == d1 || digit == d2 || digit == d3)
              continue;
            if (subset[pos]->candidates[digit] == 1)
              modified = 1;
            subset[pos]->candidates[digit] = 0;
          }
        }
      }
    }
  }
  return modified;
}

char clean_hidden_triples(Grid grid) {
  if (grid == NULL)
    return 0;
  return apply_rule_on_grid(grid, clean_hidden_triples_in_subset);
}

char is_grid_valid(Grid grid) {
  if (grid == NULL || grid->cells == NULL)
    return 0;
  /* Every unsolved cell must retain at least one candidate. */
  for (int i = 0; i < GRID_CELL_COUNT; i++) {
    if (grid->cells[i].value != 0)
      continue;
    char any = 0;
    for (int d = 0; d < CANDIDATE_COUNT; d++)
      if (grid->cells[i].candidates[d]) {
        any = 1;
        break;
      }
    if (!any)
      return 0;
  }

  for (int subsetIndex = 0; subsetIndex < SUBSET_COUNT; subsetIndex++) {
    char seen[CANDIDATE_COUNT] = {0};
    for (int cellIndex = 0; cellIndex < CELLS_PER_UNIT; cellIndex++) {
      char value = grid->subsets.items[subsetIndex][cellIndex]->value;
      if (value == 0)
        continue;
      if (seen[value - 1]) {
        return 0;
      }
      seen[value - 1] = 1;
    }
  }

  return 1;
}

static void save_grid_state(Grid grid, SolverStep *step) {
  if (grid == NULL || step == NULL)
    return;

  for (int i = 0; i < GRID_CELL_COUNT; i++) {
    step->gridValues[i] = grid->cells[i].value;
    for (int d = 0; d < GRID_SIDE; d++)
      step->gridCandidates[i][d] = grid->cells[i].candidates[d];
  }
}

static void restore_grid_state(Grid grid, const SolverStep *step) {
  if (grid == NULL || step == NULL)
    return;

  for (int i = 0; i < GRID_CELL_COUNT; i++) {
    grid->cells[i].value = step->gridValues[i];
    for (int d = 0; d < GRID_SIDE; d++)
      grid->cells[i].candidates[d] = step->gridCandidates[i][d];
  }
}

char guess_cell_value(Grid grid) {
  for (int expectedCount = 2; expectedCount <= GRID_SIDE; expectedCount++) {
    for (int i = 0; i < GRID_CELL_COUNT; i++) {
      if (grid->cells[i].value != 0)
        continue;

      int count = 0;
      int firstValue = -1;
      for (int d = 0; d < GRID_SIDE; d++) {
        if (grid->cells[i].candidates[d]) {
          count++;
          if (firstValue == -1)
            firstValue = d + 1;
        }
      }

      if (count == expectedCount) {
        if (historyIndex >= GRID_CELL_COUNT)
          return 0;
        save_grid_state(grid, &solverHistory[historyIndex]);
        set_cell_value(&grid->cells[i], (char)firstValue, 1);
        return 1;
      }
    }
  }
  return 0;
}

void reset_solver_history(void) {
  for (int i = 0; i < GRID_CELL_COUNT; i++) {
    solverHistory[i].cell = NULL;
    solverHistory[i].isGuess = 0;
    solverHistory[i].value = 0;
  }
  historyIndex = 0;
}

int count_history_guesses(void) {
  int count = 0;
  for (int i = 0; i < historyIndex; i++)
    if (solverHistory[i].isGuess)
      count++;
  return count;
}

void backtrack_last_guess(Grid grid) {
  if (grid == NULL || historyIndex <= 0)
    return;

  int stepIndex = historyIndex - 1;
  while (stepIndex >= 0 && solverHistory[stepIndex].isGuess == 0)
    stepIndex--;

  if (stepIndex < 0)
    return;

  SolverStep failedGuess = solverHistory[stepIndex];
  SudokuCell *guessedCell = failedGuess.cell;
  unsigned char guessedValue = failedGuess.value;

  restore_grid_state(grid, &failedGuess);
  if (guessedCell != NULL && guessedValue >= 1 &&
      guessedValue <= CANDIDATE_COUNT)
    guessedCell->candidates[guessedValue - 1] = 0;

  historyIndex = stepIndex;
}

void deduce_until_stable(Grid grid) {
  char modified;
  do {
    modified = 0;
    modified |= clean_grid(grid);
    modified |= solve_naked_singles(grid);
    modified |= solve_hidden_singles(grid);
    modified |= clean_grid(grid);
    modified |= clean_naked_pairs(grid);
    modified |= clean_naked_pairs(grid);
    modified |= clean_hidden_pairs(grid);
    modified |= clean_naked_triples(grid);
    modified |= clean_hidden_triples(grid);
  } while (modified);
}

char has_pending_guess(void) {
  for (int i = historyIndex - 1; i >= 0; i--)
    if (solverHistory[i].isGuess)
      return 1;
  return 0;
}

char solve_and_count_guesses(Grid grid, int *guessCount) {
  if (grid == NULL)
    return 0;
  reset_solver_history();
  reset_grid_candidates(grid);
  deductionCount = 0;
  if (guessCount != NULL)
    *guessCount = 0;

  for (long round = 0; round < MAX_SOLVE_ROUNDS; round++) {
    deduce_until_stable(grid);

    if (!is_grid_valid(grid)) {
      if (!has_pending_guess())
        return 0;
      backtrack_last_guess(grid);
      continue;
    }

    if (count_filled_cells(grid) == GRID_CELL_COUNT)
      return 1;

    if (!guess_cell_value(grid))
      return 0;
    if (guessCount != NULL)
      (*guessCount)++;
  }
  return 0;
}

char solve(Grid grid) { return solve_and_count_guesses(grid, NULL); }

char solve_with_stats(Grid grid, int *guessCount) {
  char solved = solve_and_count_guesses(grid, guessCount);
  reset_solver_history();
  return solved;
}
