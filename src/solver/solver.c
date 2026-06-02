#include "solver.h"

void solver_placeholder(void) {}

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