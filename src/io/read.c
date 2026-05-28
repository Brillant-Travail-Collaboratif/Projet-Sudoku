//
// Created by Meike Rayan on 28/05/2026.
//

#include "read.h"
Sudoku *loadSudokuFromFile(const char *filename) {

    if (filename == NULL) return NULL;

    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Sudoku *sudoku = createSudoku(9, 9);

    if (sudoku == NULL) {
        fclose(file);
        return NULL;
    }

    char buffer[256];

    for (unsigned char row = 0; row < 9; row++) {

        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file);
            return NULL;
        }

        for (unsigned char column = 0; column < 9; column++) {

            char value = buffer[column * 2];

            if (value == '?') {
                sudokuSetXY(sudoku, column, row, 0);
            }


            else if (value >= '1' && value <= '9') {
                sudokuSetXY(sudoku, column, row, value - '0');
            }

            else {
                sudokuSetXY(sudoku, column, row, 0);
            }
        }
    }

    fclose(file);

    return sudoku;
}


int saveSudokuToFile(const char *filename, const Sudoku *sudoku) {

    if ((filename == NULL) || (sudoku == NULL)) return -1;

    FILE *file = fopen(filename, "w");

    if (file == NULL) return -1;

    for (unsigned char row = 0; row < 9; row++) {

        for (unsigned char column = 0; column < 9; column++) {

            int value = sudokuGetXY(sudoku, column, row);


            if (value == 0) {
                fprintf(file, "?");
            }


            else {
                fprintf(file, "%d", value);
            }


            if (column < 8) {
                fprintf(file, " ");
            }
        }

        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}
