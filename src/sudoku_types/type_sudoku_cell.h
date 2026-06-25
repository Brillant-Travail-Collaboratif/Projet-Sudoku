#ifndef TYPE_SUDOKU_CELL_H
#define TYPE_SUDOKU_CELL_H

#define CANDIDATE_COUNT 9

typedef struct SudokuCell {
  char value;
  char candidates[CANDIDATE_COUNT];
} SudokuCell;

#endif /* TYPE_SUDOKU_CELL_H */
