#include "difficulty_internal.h"

DifficultyTarget get_difficulty_target(Difficulty difficulty) {
  switch (difficulty) {
  case TRIVIAL:
    return (DifficultyTarget){1, 40, 0, 0};
  case BASIC:
    return (DifficultyTarget){41, 50, 0, 0};
  case INTERMEDIATE:
    return (DifficultyTarget){51, 64, 0, 0};
  case DIFFICULT:
    return (DifficultyTarget){1, MAX_REMOVED_FOR_GUESS_LEVELS, 1, 5};
  case EXPERT:
    return (DifficultyTarget){1, MAX_REMOVED_FOR_GUESS_LEVELS, 6, INT_MAX};
  }
  return (DifficultyTarget){41, 50, 0, 0};
}

const char *get_difficulty_name(Difficulty difficulty) {
  switch (difficulty) {
  case TRIVIAL:
    return "trivial";
  case BASIC:
    return "basic";
  case INTERMEDIATE:
    return "intermediate";
  case DIFFICULT:
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
    *difficulty = DIFFICULT;
    return 1;
  }
  if (strcmp(text, "expert") == 0) {
    *difficulty = EXPERT;
    return 1;
  }
  return 0;
}

char measure_difficulty(Grid puzzle, int *guesses) {
  Grid check = clone_grid_values(puzzle);
  if (check == NULL)
    return 0;

  char solved = solve_with_stats(check, guesses);
  delete_grid(check);
  return solved;
}

char matches_difficulty(Grid puzzle, Difficulty difficulty) {
  DifficultyTarget target = get_difficulty_target(difficulty);
  int removed = count_removed_cells(puzzle);
  int guesses = 0;

  if (removed < target.minRemoved || removed > target.maxRemoved)
    return 0;
  if (!measure_difficulty(puzzle, &guesses))
    return 0;
  if (guesses < target.minGuesses ||
      guesses > target.maxGuesses)
    return 0;

  return 1;
}

int count_removed_cells(Grid grid) { return GRID_CELL_COUNT - count_filled_cells(grid); }