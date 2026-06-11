#include "generator_internal.h"

char is_value_allowed(Grid grid, int index, char value) {
  int row = index / GRID_SIDE;
  int col = index % GRID_SIDE;
  int box_row = (row / 3) * 3;
  int box_col = (col / 3) * 3;

  for (int c = 0; c < GRID_SIDE; c++)
    if (grid->allTiles[row * GRID_SIDE + c].value == value)
      return 0;

  for (int r = 0; r < GRID_SIDE; r++)
    if (grid->allTiles[r * GRID_SIDE + col].value == value)
      return 0;

  for (int r = box_row; r < box_row + 3; r++)
    for (int c = box_col; c < box_col + 3; c++)
      if (grid->allTiles[r * GRID_SIDE + c].value == value)
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
  int best_index = -1;
  int best_count = GRID_SIDE + 1;

  for (int i = 0; i < GRID_SIZE; i++) {
    if (grid->allTiles[i].value != 0)
      continue;

    char values[GRID_SIDE];
    int count = collect_allowed_values(grid, i, values);
    if (count < best_count) {
      best_count = count;
      best_index = i;
      if (count <= 1)
        break;
    }
  }

  return best_index;
}

char fill_complete_grid(Grid grid, GeneratorRandom *random) {
  int index = find_best_empty_cell(grid);
  if (index < 0)
    return is_grid_valid(grid);

  char values[GRID_SIDE];
  int count = collect_allowed_values(grid, index, values);
  int order[GRID_SIDE];
  for (int i = 0; i < count; i++)
    order[i] = i;
  shuffle_ints(random, order, count);

  for (int i = 0; i < count; i++) {
    grid->allTiles[index].value = values[order[i]];
    reset_grid_candidates(grid);
    if (fill_complete_grid(grid, random))
      return 1;
    grid->allTiles[index].value = 0;
    reset_grid_candidates(grid);
  }

  return 0;
}

char grids_have_same_values(Grid a, Grid b) {
  if (a == NULL || b == NULL || a->allTiles == NULL || b->allTiles == NULL)
    return 0;

  for (int i = 0; i < GRID_SIZE; i++)
    if (a->allTiles[i].value != b->allTiles[i].value)
      return 0;
  return 1;
}

char solution_matches_completed_grid(Grid puzzle, Grid completed) {
  Grid solved = clone_grid_values(puzzle);
  if (solved == NULL)
    return 0;

  int suppositions = 0;
  char ok = solve_with_stats(solved, &suppositions);
  (void)suppositions;
  ok = ok && grids_have_same_values(solved, completed);
  delete_grid(solved);
  return ok;
}

char count_solutions(Grid grid, int index, int *count, Grid first_solution) {
  if (*count > 1)
    return 1;

  if (index == GRID_SIZE) {
    (*count)++;
    if (*count == 1 && first_solution != NULL) {
      for (int i = 0; i < GRID_SIZE; i++)
        first_solution->allTiles[i].value = grid->allTiles[i].value;
      reset_grid_candidates(first_solution);
    }
    return 1;
  }

  if (grid->allTiles[index].value != 0)
    return count_solutions(grid, index + 1, count, first_solution);

  char values[GRID_SIDE];
  int value_count = collect_allowed_values(grid, index, values);
  for (int i = 0; i < value_count; i++) {
    grid->allTiles[index].value = values[i];
    if (!count_solutions(grid, index + 1, count, first_solution))
      return 0;
    if (*count > 1)
      break;
  }
  grid->allTiles[index].value = 0;
  return 1;
}

char has_unique_matching_solution(Grid puzzle, Grid completed) {
  Grid check = clone_grid_values(puzzle);
  Grid first_solution = create_grid();
  int solutions = 0;
  char ok = 0;

  if (check != NULL && first_solution != NULL && is_grid_valid(check) &&
      count_solutions(check, 0, &solutions, first_solution)) {
    ok = solutions == 1 && grids_have_same_values(first_solution, completed) &&
         solution_matches_completed_grid(puzzle, completed);
  }

  delete_grid(first_solution);
  delete_grid(check);
  return ok;
}

int target_removed_count(Difficulty difficulty, GeneratorRandom *random) {
  if (difficulty == DIFFICILE)
    return 50 + generator_random_int(random, 15);
  if (difficulty == EXPERT)
    return 58 + generator_random_int(random, 13);

  DifficultyTarget target = target_for_difficulty(difficulty);
  int span = target.max_removed - target.min_removed + 1;
  return target.min_removed + generator_random_int(random, span);
}

Grid build_candidate_puzzle(Grid completed, Difficulty difficulty,
                            GeneratorRandom *random) {
  Grid puzzle = clone_grid_values(completed);
  if (puzzle == NULL)
    return NULL;

  int target = target_removed_count(difficulty, random);
  int order[GRID_SIZE];
  for (int i = 0; i < GRID_SIZE; i++)
    order[i] = i;
  shuffle_ints(random, order, GRID_SIZE);

  for (int i = 0; i < GRID_SIZE && removed_count(puzzle) < target; i++) {
    int index = order[i];
    char old_value = puzzle->allTiles[index].value;
    if (old_value == 0)
      continue;

    puzzle->allTiles[index].value = 0;
    reset_grid_candidates(puzzle);
    if (!has_unique_matching_solution(puzzle, completed)) {
      puzzle->allTiles[index].value = old_value;
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
  GeneratorRandom random = {seed == 0 ? 1u : seed};

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