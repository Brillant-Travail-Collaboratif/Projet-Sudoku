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

char solveHiddenSinglesInSubset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char modified = 0;
  for (char j = 1; j <= TILES_PER_LINE; j++) {
    char alreadyPlaced = 0;
    unsigned char count = 0;
    unsigned char target = 0;

    for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
      if (subset[i]->value == j) {
        alreadyPlaced = 1;
        break;
      }
      if (subset[i]->value == 0 && subset[i]->possible[j - 1]) {
        count++;
        target = i;
      }
    }
    if (!alreadyPlaced && count == 1) {
      fixTileValue(subset[target], j);
      modified = 1;
    }
  }
  return modified;
}

char solve_hidden_singles(Grid grid) {
  if (grid == NULL)
    return 0;

  AllSubsets *all = malloc(sizeof(AllSubsets));
  if (all == NULL)
    return 0;

  if (buildAllSubsets(grid, all) != 0)
    return 0;

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (solveHiddenSinglesInSubset(all->subsets[i]) != 0)
      modified = 1;
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

char cleanSubset(Subset s) {
  if (s == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
    const char value = s[i]->value;
    if (value == 0)
      continue; /* case inconnue : rien a propager */

    for (unsigned char j = 0; j < TILES_PER_LINE; j++) {
      if (j == i)
        continue;
      if (s[j]->value != 0)
        continue;
      if (removeCandidate(s[j], value))
        modified = 1;
    }
  }
  return modified;
}
char solve_hidden_singles_in_line(SudokuTile *line) {
  unsigned char modified = 0;
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
    if (localisationOfCandidate != 0) {
      tileSetValue(&line[localisationOfCandidate], i);
      modified = 1;
    }
  }
  return modified;
}

char clean_grid(Grid grid) {
  if (grid == NULL)
    return 0;

  AllSubsets *all = malloc(sizeof(AllSubsets));
  if (all == NULL)
    return 0;

  if (buildAllSubsets(grid, all) != 0)
    return 0;

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (cleanSubset(all->subsets[i]) != 0)
      modified = 1;
  }

  return modified;
}

/* Versions grille : on boucle sur les 27 sous ensembles. */

static char apply_rule_on_grid(Grid grid, char (*rule)(Subset)) {
  if (grid == NULL || rule == NULL)
    return 0;
  AllSubsets *all = malloc(sizeof(AllSubsets));
  if (all == NULL)
    return 0;
  if (buildAllSubsets(grid, all) != 0) {
    free(all);
    return 0;
  }
  char modified = 0;
  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (rule(all->subsets[i]))
      modified = 1;
  }
  freeAllSubsets(all);
  free(all);
  return modified;
}

/* Liste les candidats restants d'une case dans out[] (chiffres 1..9).Retourne
 * le nombre de candidats. */

static unsigned char list_candidates(SudokuTile *t,
                                     char out[NUMBER_OF_POSSIBLE]) {
  unsigned char n = 0;
  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++) {
    if (t->possible[d])
      out[n++] = (char)(d + 1);
  }
  return n;
}

char clean_naked_pair_in_subset(Subset s) {
  if (s == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
    if (s[i]->value != 0)
      continue;
    char ci[NUMBER_OF_POSSIBLE];
    if (list_candidates(s[i], ci) != 2)
      continue;

    for (unsigned char j = i + 1; j < TILES_PER_LINE; j++) {
      if (s[j]->value != 0)
        continue;
      char cj[NUMBER_OF_POSSIBLE];
      if (list_candidates(s[j], cj) != 2)
        continue;
      if (ci[0] != cj[0] || ci[1] != cj[1])
        continue;

      /* Quand on trouve une paire , on nettoie les autres cases inconnues du
       * subset. */
      for (unsigned char k = 0; k < TILES_PER_LINE; k++) {
        if (k == i || k == j || s[k]->value != 0)
          continue;
        if (removeCandidate(s[k], ci[0]))
          modified = 1;
        if (removeCandidate(s[k], ci[1]))
          modified = 1;
      }
    }
  }
  return modified;
}

char clean_naked_pairs(Grid grid) {
  return apply_rule_on_grid(grid, clean_naked_pair_in_subset);
}

char clean_hidden_pairs_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char candidates[NUMBER_OF_POSSIBLE][3];

  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++) {
    candidates[d][0] = 0;
    candidates[d][1] = 0;
    candidates[d][2] = 0;
  }

  for (unsigned char digit = 0; digit < NUMBER_OF_POSSIBLE; digit++) {
    for (unsigned char pos = 0; pos < NUMBER_OF_POSSIBLE; pos++) {
      if (subset[pos]->possible[digit] != 0) {
        if (candidates[digit][2] < 2) {
          candidates[digit][candidates[digit][2]] = pos;
        }
        candidates[digit][2]++;
      }
    }
  }

  unsigned char modified = 0;

  for (unsigned char d1 = 0; d1 < NUMBER_OF_POSSIBLE; d1++) {
    if (candidates[d1][2] != 2)
      continue;

    for (unsigned char d2 = d1 + 1; d2 < NUMBER_OF_POSSIBLE; d2++) {
      if (candidates[d2][2] != 2)
        continue;

      unsigned char a1 = candidates[d1][0];
      unsigned char a2 = candidates[d1][1];
      unsigned char b1 = candidates[d2][0];
      unsigned char b2 = candidates[d2][1];

      if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) {
        for (unsigned char digit = 0; digit < NUMBER_OF_POSSIBLE; digit++) {
          if (digit != d1 && digit != d2) {
            if (subset[a1]->possible[digit] == 1 ||
                subset[a2]->possible[digit] == 1)
              modified = 1;
            subset[a1]->possible[digit] = 0;
            subset[a2]->possible[digit] = 0;
          }
        }
      }
    }
  }

  return modified;
}

char clean_hidden_pairs(Grid grid) {
  if (grid == NULL)
    return 0;

  return apply_rule_on_grid(grid, clean_hidden_pairs_in_subset);
}