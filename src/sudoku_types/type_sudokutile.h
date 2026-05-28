#ifndef TYPE_SUDOKUTILE_H
#define TYPE_SUDOKUTILE_H

#define NUMBER_OF_POSSIBLE 9

typedef struct SudokuTile {
  char value;
  char possible[NUMBER_OF_POSSIBLE];
} SudokuTile;

#endif /* TYPE_SUDOKUTILE_H */
