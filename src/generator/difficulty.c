#include "difficulty_internal.h"

DifficultyTarget target_for_difficulty(Difficulty difficulty) {
  switch (difficulty) {
  case TRIVIAL:
    return (DifficultyTarget){1, 40, 0, 0};
  case BASIC:
    return (DifficultyTarget){41, 50, 0, 0};
  case INTERMEDIATE:
    return (DifficultyTarget){51, 64, 0, 0};
  case DIFFICILE:
    return (DifficultyTarget){1, MAX_REMOVED_FOR_GUESSED_LEVELS, 1, 5};
  case EXPERT:
    return (DifficultyTarget){1, MAX_REMOVED_FOR_GUESSED_LEVELS, 6, INT_MAX};
  }
  return (DifficultyTarget){41, 50, 0, 0};
}

const char *difficulty_to_string(Difficulty difficulty) {
  switch (difficulty) {
  case TRIVIAL:
    return "trivial";
  case BASIC:
    return "basic";
  case INTERMEDIATE:
    return "intermediate";
  case DIFFICILE:
    return "difficile";
  case EXPERT:
    return "expert";
  }
  return "basic";
}

char parse_difficulty(const char *text, Difficulty *difficulty) {
  if (text == NULL || difficulty == NULL)
    return 0;

  if (strcmp(text, "trivial") == 0) {
    *difficulty = TRIVIAL;
    return 1;
  }
  if (strcmp(text, "basic") == 0) {
    *difficulty = BASIC;
    return 1;
  }
  if (strcmp(text, "intermediate") == 0) {
    *difficulty = INTERMEDIATE;
    return 1;
  }
  if (strcmp(text, "difficile") == 0 || strcmp(text, "difficult") == 0) {
    *difficulty = DIFFICILE;
    return 1;
  }
  if (strcmp(text, "expert") == 0) {
    *difficulty = EXPERT;
    return 1;
  }
  return 0;
}

char measure_difficulty(Grid puzzle, int *suppositions) {
  Grid check = clone_grid_values(puzzle);
  if (check == NULL)
    return 0;

  char solved = solve_with_stats(check, suppositions);
  delete_grid(check);
  return solved;
}

char matches_difficulty(Grid puzzle, Difficulty difficulty) {
  DifficultyTarget target = target_for_difficulty(difficulty);
  int removed = removed_count(puzzle);
  int suppositions = 0;

  if (removed < target.min_removed || removed > target.max_removed)
    return 0;
  if (!measure_difficulty(puzzle, &suppositions))
    return 0;
  if (suppositions < target.min_suppositions ||
      suppositions > target.max_suppositions)
    return 0;

  return 1;
}

int removed_count(Grid grid) {
  return GRID_SIZE - grid_filled_count(grid);
}