#ifndef TYPE_DIFFICULTY_H
#define TYPE_DIFFICULTY_H

typedef struct DifficultyTarget {
  int min_removed;
  int max_removed;
  int min_suppositions;
  int max_suppositions;
} DifficultyTarget;

typedef enum Difficulty {
  TRIVIAL,
  BASIC,
  INTERMEDIATE,
  DIFFICILE,
  EXPERT
} Difficulty;

#endif /* TYPE_DIFFICULTY_H */