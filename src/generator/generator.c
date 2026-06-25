#include "generator_internal.h"

char is_value_allowed(Grid grid, int index, char value) {
  int row = index / GRID_SIDE;
  int column = index % GRID_SIDE;
  int boxRow = (row / 3) * 3;
  int boxColumn = (column / 3) * 3;

  for (int currentColumn = 0; currentColumn < GRID_SIDE; currentColumn++)
    if (grid->cells[row * GRID_SIDE + currentColumn].value == value)
      return 0;

  for (int currentRow = 0; currentRow < GRID_SIDE; currentRow++)
    if (grid->cells[currentRow * GRID_SIDE + column].value == value)
      return 0;

  for (int currentRow = boxRow; currentRow < boxRow + 3; currentRow++)
    for (int currentColumn = boxColumn; currentColumn < boxColumn + 3;
         currentColumn++)
      if (grid->cells[currentRow * GRID_SIDE + currentColumn].value == value)
        return 0;

  return 1;
}

int collect_allowed_values(Grid grid, int index, char values[GRID_SIDE]) {
  int count = 0;
  for (char value = 1; value <= GRID_SIDE; value++)
    if (is_value_allowed(grid, index, value))
      values[count++] = value;
  return count;
}

int find_best_empty_cell(Grid grid) {
  int bestIndex = -1;
  int bestCount = GRID_SIDE + 1;

  for (int i = 0; i < GRID_CELL_COUNT; i++) {
    if (grid->cells[i].value != 0)
      continue;

    char values[GRID_SIDE];
    int count = collect_allowed_values(grid, i, values);
    if (count < bestCount) {
      bestCount = count;
      bestIndex = i;
      if (count <= 1)
        break;
    }
  }

  return bestIndex;
}

char fill_complete_grid(Grid grid, RandomGenerator *random) {
  int index = find_best_empty_cell(grid);
  if (index < 0)
    return is_grid_valid(grid);

  char values[GRID_SIDE];
  int count = collect_allowed_values(grid, index, values);
  int order[GRID_SIDE];
  for (int i = 0; i < count; i++)
    order[i] = i;
  shuffle_integers(random, order, count);

  for (int i = 0; i < count; i++) {
    grid->cells[index].value = values[order[i]];
    reset_grid_candidates(grid);
    if (fill_complete_grid(grid, random))
      return 1;
    grid->cells[index].value = 0;
    reset_grid_candidates(grid);
  }

  return 0;
}

char have_same_grid_values(Grid firstGrid, Grid secondGrid) {
  if (firstGrid == NULL || secondGrid == NULL || firstGrid->cells == NULL ||
      secondGrid->cells == NULL)
    return 0;

  for (int i = 0; i < GRID_CELL_COUNT; i++)
    if (firstGrid->cells[i].value != secondGrid->cells[i].value)
      return 0;
  return 1;
}

char does_solution_match_completed_grid(Grid puzzle, Grid completed) {
  Grid solved = clone_grid_values(puzzle);
  if (solved == NULL)
    return 0;

  int guesses = 0;
  char ok = solve_with_stats(solved, &guesses);
  (void)guesses;
  ok = ok && have_same_grid_values(solved, completed);
  delete_grid(solved);
  return ok;
}

char count_solutions(Grid grid, int index, int *count, Grid firstSolution) {
  if (*count > 1)
    return 1;

  if (index == GRID_CELL_COUNT) {
    (*count)++;
    if (*count == 1 && firstSolution != NULL) {
      for (int i = 0; i < GRID_CELL_COUNT; i++)
        firstSolution->cells[i].value = grid->cells[i].value;
      reset_grid_candidates(firstSolution);
    }
    return 1;
  }

  if (grid->cells[index].value != 0)
    return count_solutions(grid, index + 1, count, firstSolution);

  char values[GRID_SIDE];
  int valueCount = collect_allowed_values(grid, index, values);
  for (int i = 0; i < valueCount; i++) {
    grid->cells[index].value = values[i];
    if (!count_solutions(grid, index + 1, count, firstSolution))
      return 0;
    if (*count > 1)
      break;
  }
  grid->cells[index].value = 0;
  return 1;
}

char has_unique_matching_solution(Grid puzzle, Grid completed) {
  Grid check = clone_grid_values(puzzle);
  Grid firstSolution = create_grid();
  int solutions = 0;
  char ok = 0;

  if (check != NULL && firstSolution != NULL && is_grid_valid(check) &&
      count_solutions(check, 0, &solutions, firstSolution)) {
    ok = solutions == 1 && have_same_grid_values(firstSolution, completed) &&
         does_solution_match_completed_grid(puzzle, completed);
  }

  delete_grid(firstSolution);
  delete_grid(check);
  return ok;
}

int choose_removed_cell_target(Difficulty difficulty, RandomGenerator *random) {
  if (difficulty == DIFFICULT)
    return 50 + generate_random_int(random, 15);
  if (difficulty == EXPERT)
    return 58 + generate_random_int(random, 13);

  DifficultyTarget target = get_difficulty_target(difficulty);
  int span = target.maxRemoved - target.minRemoved + 1;
  return target.minRemoved + generate_random_int(random, span);
}

Grid build_candidate_puzzle(Grid completed, Difficulty difficulty,
                            RandomGenerator *random) {
  Grid puzzle = clone_grid_values(completed);
  if (puzzle == NULL)
    return NULL;

  int target = choose_removed_cell_target(difficulty, random);
  int order[GRID_CELL_COUNT];
  for (int i = 0; i < GRID_CELL_COUNT; i++)
    order[i] = i;
  shuffle_integers(random, order, GRID_CELL_COUNT);

  for (int i = 0; i < GRID_CELL_COUNT && count_removed_cells(puzzle) < target; i++) {
    int index = order[i];
    char oldValue = puzzle->cells[index].value;
    if (oldValue == 0)
      continue;

    puzzle->cells[index].value = 0;
    reset_grid_candidates(puzzle);
    if (!has_unique_matching_solution(puzzle, completed)) {
      puzzle->cells[index].value = oldValue;
      reset_grid_candidates(puzzle);
    }
  }

  if (!matches_difficulty(puzzle, difficulty)) {
    delete_grid(puzzle);
    return NULL;
  }

  return puzzle;
}

Grid generate_sudoku(Difficulty difficulty, unsigned int seed) {
  RandomGenerator random = {seed == 0 ? 1u : seed};

  for (int attempt = 0; attempt < MAX_GENERATION_ATTEMPTS; attempt++) {
    Grid completed = create_grid();
    if (completed == NULL)
      return NULL;

    if (!fill_complete_grid(completed, &random)) {
      delete_grid(completed);
      continue;
    }

    Grid puzzle = build_candidate_puzzle(completed, difficulty, &random);
    delete_grid(completed);
    if (puzzle != NULL)
      return puzzle;
  }

  return NULL;
}
