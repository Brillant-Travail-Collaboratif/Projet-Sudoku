#ifndef TYPE_DIFFICULTY_H
#define TYPE_DIFFICULTY_H

typedef struct DifficultyTarget {
  int minRemoved;
  int maxRemoved;
  int minGuesses;
  int maxGuesses;
} DifficultyTarget;

typedef enum Difficulty {
  TRIVIAL,
  BASIC,
  INTERMEDIATE,
  DIFFICULT,
  EXPERT
} Difficulty;

#endif /* TYPE_DIFFICULTY_H */