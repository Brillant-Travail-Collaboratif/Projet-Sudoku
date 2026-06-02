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

char cleanLine(SudokuTile *line) {
  if (line == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
    const char value = line[i].value;
    if (value == 0)
      continue; /* case inconnue : rien a propager */

    /* On retire cette valeur des candidats des autres cases inconnues. */
    for (unsigned char j = 0; j < TILES_PER_LINE; j++) {
      if (j == i)
        continue;
      if (line[j].value != 0)
        continue;
      if (removeCandidate(&line[j], value))
        modified = 1;
    }
  }
  return modified;
}

char solve_hidden_singles_in_line(SudokuTile *line) {
  for (unsigned char i = 1; i <= NUMBER_OF_POSSIBLE; i++) {
    unsigned char localisationOfCandidate = 0;
    for (unsigned char j = 0; j < NUMBER_OF_POSSIBLE; j++) {
      char candidateValue = tileGetPossible(&line[j])[i];
      if (candidateValue != 0 && localisationOfCandidate != 0)
        localisationOfCandidate = j;
      else if (candidateValue != 0) {
        localisationOfCandidate = 0;
        break;
      }
    }
    if (localisationOfCandidate != 0)
      tileSetValue(&line[localisationOfCandidate], i);
  }
}