#ifndef CELL_H
#define CELL_H

#include "../sudoku_types/type_sudoku_cell.h"

SudokuCell *create_cell(const char value,
                        const char candidates[CANDIDATE_COUNT]);
void delete_cell(SudokuCell *cell);

char get_cell_value(SudokuCell *cell);
char *get_cell_candidates(SudokuCell *cell);
char set_cell_value(SudokuCell *cell, char value, unsigned char isGuess);
char set_cell_candidates(SudokuCell *cell, char candidates[CANDIDATE_COUNT]);

#endif /* CELL_H */
