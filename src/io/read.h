//
// Created by Meike Rayan on 28/05/2026.
//

#ifndef PROJET_SUDOKU_READ_H
#define PROJET_SUDOKU_READ_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../sudoku_types/sudoku_types.h"

Sudoku *loadSudokuFromFile(const char *filename);

int saveSudokuToFile(const char *filename, const Sudoku *sudoku);
#endif //PROJET_SUDOKU_READ_H
