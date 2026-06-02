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
    if (line == NULL) return 0;

    char modified = 0;
    for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
        const char value = line[i].value;
        if (value == 0) continue; /* case inconnue : rien a propager */

        /* On retire cette valeur des candidats des autres cases inconnues. */
        for (unsigned char j = 0; j < TILES_PER_LINE; j++) {
            if (j == i) continue;
            if (line[j].value != 0) continue;
            if (removeCandidate(&line[j], value)) modified = 1;
        }
    }
    return modified;
}

}