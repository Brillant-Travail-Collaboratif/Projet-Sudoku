#ifndef SUDOKU_TYPES_H
#define SUDOKU_TYPES_H
typedef struct {

    int width;
    int height;

    int grid[9][9];

} Sudoku;



Sudoku *createSudoku(int width, int height);



void sudokuSetXY(Sudoku *sudoku, int x, int y, int value);



int sudokuGetXY(const Sudoku *sudoku, int x, int y);

#endif /* SUDOKU_TYPES_H */
