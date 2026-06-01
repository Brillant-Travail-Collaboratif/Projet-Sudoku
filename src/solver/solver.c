#include "solver.h"

void solver_placeholder(void) {}
#include "solver_internal.h"

char removeCandidate(SudokuTile *tile, char value) {
    if (tile->possible[value - 1]) {
        tile->possible[value - 1] = 0;
        return 1;
    }
    return 0;
}

char cleanLine(SudokuTile *line) {

}