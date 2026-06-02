#include "solver_internal.h"

unsigned char countCandidates(const SudokuTile *tile, char *candidate) {
  unsigned char count = 0;
  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++) {
    if (tile->possible[d]) {
      count++;
      *candidate = (char)(d + 1);
    }
  }
  return count;
}

void fixTileValue(SudokuTile *tile, char value) {
  tile->value = value;
  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++)
    tile->possible[d] = 0;
  tile->possible[value - 1] = 1;
}

char solveNakedSingles(Grid grid) {
  if (grid == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    /* On ne touche pas aux cases deja fixees. */
    if (grid[i].value != 0)
      continue;

    char candidate = 0;
    const unsigned char count = countCandidates(&grid[i], &candidate);
    if (count == 1) {
      fixTileValue(&grid[i], candidate);
      modified = 1;
    }
  }
  return modified;
}

char removeCandidate(SudokuTile *tile, char value) {
  if (tile->possible[value - 1]) {
    tile->possible[value - 1] = 0;
    return 1;
  }
  return 0;
}

char cleanLine(SudokuTile *line) {}