#include "solver.h"
#include "solver_internal.h"
#include <stddef.h>

unsigned char count_candidates(const SudokuTile *tile, char *candidate) {
  unsigned char count = 0;
  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++) {
    if (tile->possible[d]) {
      count++;
      *candidate = (char)(d + 1);
    }
  }
  return count;
}

char solve_naked_singles(Grid grid) {
  if (grid == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    /* On ne touche pas aux cases deja fixees. */
    if (grid[i].value != 0)
      continue;

    char candidate = 0;
    const unsigned char count = count_candidates(&grid[i], &candidate);
    if (count == 1) {
      set_tile_value(&grid[i], candidate, 0);
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
      set_tile_value(subset[target], j, 0);
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

  if (build_all_subsets(grid, all) != 0) {
    free(all);
    return 0;
  }

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (solveHiddenSinglesInSubset(all->subsets[i]) != 0)
      modified = 1;
  }

  free_all_subsets(all);
  free(all);
  return modified;
}

char remove_tile_possible(SudokuTile *tile, char value) {
  if (tile->possible[value - 1]) {
    tile->possible[value - 1] = 0;
    return 1;
  }
  return 0;
}

char clean_line(SudokuTile *line) {
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
      if (remove_tile_possible(&line[j], value))
        modified = 1;
    }
  }
  return modified;
}

char clean_subset(Subset s) {
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
      if (remove_tile_possible(s[j], value))
        modified = 1;
    }
  }
  return modified;
}
char solve_hidden_singles_in_line(SudokuTile *line) {
  if (line == NULL)
    return 0;

  unsigned char modified = 0;
  for (unsigned char i = 0; i < NUMBER_OF_POSSIBLE; i++) {
    unsigned char localisationOfCandidate = 0;
    for (unsigned char j = 0; j < NUMBER_OF_POSSIBLE; j++) {
      char *possible = get_tile_possibles(&line[j]);
      if (possible == NULL)
        continue;
      char candidateValue = possible[i];
      if (candidateValue != 0 && localisationOfCandidate != 0)
        localisationOfCandidate = j;
      else if (candidateValue != 0) {
        localisationOfCandidate = 0;
        break;
      }
    }
    if (localisationOfCandidate != 0) {
      set_tile_value(&line[localisationOfCandidate], i + 1, 0);
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

  if (build_all_subsets(grid, all) != 0) {
    free(all);
    return 0;
  }

  unsigned char modified = 0;

  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (clean_subset(all->subsets[i]) != 0)
      modified = 1;
  }

  free_all_subsets(all);
  free(all);
  return modified;
}

/* Versions grille : on boucle sur les 27 sous ensembles. */

char apply_rule_on_grid(Grid grid, char (*rule)(Subset)) {
  if (grid == NULL || rule == NULL)
    return 0;
  AllSubsets *all = malloc(sizeof(AllSubsets));
  if (all == NULL)
    return 0;
  if (build_all_subsets(grid, all) != 0) {
    free(all);
    return 0;
  }
  char modified = 0;
  for (unsigned char i = 0; i < SUBSET_COUNT; i++) {
    if (rule(all->subsets[i]))
      modified = 1;
  }
  free_all_subsets(all);
  free(all);
  return modified;
}

/* Liste les candidats restants d'une case dans out[] (chiffres 1..9).Retourne
 * le nombre de candidats. */

unsigned char list_candidates(SudokuTile *t, char out[NUMBER_OF_POSSIBLE]) {
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
        if (remove_tile_possible(s[k], ci[0]))
          modified = 1;
        if (remove_tile_possible(s[k], ci[1]))
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

char clean_naked_triple_in_subset(Subset s) {
  if (s == NULL)
    return 0;

  char modified = 0;
  for (unsigned char i = 0; i < TILES_PER_LINE; i++) {
    if (s[i]->value != 0)
      continue;
    char ci[NUMBER_OF_POSSIBLE];
    int ni = list_candidates(s[i], ci);
    if (ni < 2 || ni > 3)
      continue;

    for (unsigned char j = i + 1; j < TILES_PER_LINE; j++) {
      if (s[j]->value != 0)
        continue;
      char cj[NUMBER_OF_POSSIBLE];
      int nj = list_candidates(s[j], cj);
      if (nj < 2 || nj > 3)
        continue;

      for (unsigned char k = j + 1; k < TILES_PER_LINE; k++) {
        if (s[k]->value != 0)
          continue;
        char ck[NUMBER_OF_POSSIBLE];
        int nk = list_candidates(s[k], ck);
        if (nk < 2 || nk > 3)
          continue;

        char union_vals[NUMBER_OF_POSSIBLE];
        int union_size = 0;

        for (int a = 0; a < ni; a++) {
          char found = 0;
          for (int b = 0; b < union_size; b++)
            if (union_vals[b] == ci[a]) {
              found = 1;
              break;
            }
          if (!found)
            union_vals[union_size++] = ci[a];
        }

        for (int a = 0; a < nj; a++) {
          char found = 0;
          for (int b = 0; b < union_size; b++)
            if (union_vals[b] == cj[a]) {
              found = 1;
              break;
            }
          if (!found)
            union_vals[union_size++] = cj[a];
        }

        for (int a = 0; a < nk; a++) {
          char found = 0;
          for (int b = 0; b < union_size; b++)
            if (union_vals[b] == ck[a]) {
              found = 1;
              break;
            }
          if (!found)
            union_vals[union_size++] = ck[a];
        }

        if (union_size != 3)
          continue;

        for (unsigned char l = 0; l < TILES_PER_LINE; l++) {
          if (l == i || l == j || l == k || s[l]->value != 0)
            continue;
          for (int a = 0; a < union_size; a++)
            if (remove_tile_possible(s[l], union_vals[a]))
              modified = 1;
        }
      }
    }
  }
  return modified;
}

char clean_naked_triples(Grid grid) {
  return apply_rule_on_grid(grid, clean_naked_triple_in_subset);
}

char clean_hidden_triples_in_subset(Subset subset) {
  if (subset == NULL)
    return 0;

  unsigned char candidates[NUMBER_OF_POSSIBLE][4];

  for (unsigned char d = 0; d < NUMBER_OF_POSSIBLE; d++) {
    candidates[d][0] = 0;
    candidates[d][1] = 0;
    candidates[d][2] = 0;
    candidates[d][3] = 0;
  }

  for (unsigned char digit = 0; digit < NUMBER_OF_POSSIBLE; digit++) {
    for (unsigned char pos = 0; pos < TILES_PER_LINE; pos++) {
      if (subset[pos]->possible[digit] != 0) {
        if (candidates[digit][3] < 3)
          candidates[digit][candidates[digit][3]] = pos;
        candidates[digit][3]++;
      }
    }
  }

  char modified = 0;

  for (unsigned char d1 = 0; d1 < NUMBER_OF_POSSIBLE; d1++) {
    if (candidates[d1][3] < 2 || candidates[d1][3] > 3)
      continue;

    for (unsigned char d2 = d1 + 1; d2 < NUMBER_OF_POSSIBLE; d2++) {
      if (candidates[d2][3] < 2 || candidates[d2][3] > 3)
        continue;

      for (unsigned char d3 = d2 + 1; d3 < NUMBER_OF_POSSIBLE; d3++) {
        if (candidates[d3][3] < 2 || candidates[d3][3] > 3)
          continue;

        unsigned char union_pos[3];
        unsigned char union_size = 0;

        for (unsigned char a = 0; a < candidates[d1][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d1][a]) {
              found = 1;
              break;
            }
          if (!found && union_size < 3)
            union_pos[union_size++] = candidates[d1][a];
        }

        for (unsigned char a = 0; a < candidates[d2][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d2][a]) {
              found = 1;
              break;
            }
          if (!found && union_size < 3)
            union_pos[union_size++] = candidates[d2][a];
        }

        for (unsigned char a = 0; a < candidates[d3][3] && a < 3; a++) {
          unsigned char found = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d3][a]) {
              found = 1;
              break;
            }
          if (!found && union_size < 3)
            union_pos[union_size++] = candidates[d3][a];
        }

        if (union_size != 3)
          continue;

        unsigned char overflow = 0;
        if (candidates[d1][3] > 3 || candidates[d2][3] > 3 ||
            candidates[d3][3] > 3)
          overflow = 1;

        for (unsigned char a = 0; a < candidates[d1][3] && !overflow; a++) {
          unsigned char in_union = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d1][a]) {
              in_union = 1;
              break;
            }
          if (!in_union)
            overflow = 1;
        }
        for (unsigned char a = 0; a < candidates[d2][3] && !overflow; a++) {
          unsigned char in_union = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d2][a]) {
              in_union = 1;
              break;
            }
          if (!in_union)
            overflow = 1;
        }
        for (unsigned char a = 0; a < candidates[d3][3] && !overflow; a++) {
          unsigned char in_union = 0;
          for (unsigned char b = 0; b < union_size; b++)
            if (union_pos[b] == candidates[d3][a]) {
              in_union = 1;
              break;
            }
          if (!in_union)
            overflow = 1;
        }
        if (overflow)
          continue;

        for (unsigned char p = 0; p < union_size; p++) {
          unsigned char pos = union_pos[p];
          for (unsigned char digit = 0; digit < NUMBER_OF_POSSIBLE; digit++) {
            if (digit == d1 || digit == d2 || digit == d3)
              continue;
            if (subset[pos]->possible[digit] == 1)
              modified = 1;
            subset[pos]->possible[digit] = 0;
          }
        }
      }
    }
  }
  return modified;
}

char clean_hidden_triples(Grid grid) {
  if (grid == NULL)
    return 0;
  return apply_rule_on_grid(grid, clean_hidden_triples_in_subset);
}

char is_grid_valid(Grid grid) {
  if (grid == NULL)
    return 0;
  /* (a) Aucune case inconnue ne doit etre privee de candidats. */
  for (int i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++) {
    if (grid[i].value != 0)
      continue;
    char any = 0;
    for (int d = 0; d < NUMBER_OF_POSSIBLE; d++)
      if (grid[i].possible[d]) {
        any = 1;
        break;
      }
    if (!any)
      return 0;
  }
  /* (b) Aucun sous-ensemble ne doit contenir deux fois la meme valeur fixee.
     Ce cas n'arrive jamais en deduction propre, mais peut survenir apres
     une supposition fausse propagee : on doit le detecter pour declencher
     back_play() au lieu de retourner une grille complete et incorrecte. */
  AllSubsets *all = malloc(sizeof(AllSubsets));
  if (all == NULL)
    return 1; /* en cas de OOM, ne pas mentir : autorise */
  if (build_all_subsets(grid, all) != 0) {
    free(all);
    return 1;
  }
  for (int s = 0; s < SUBSET_COUNT; s++) {
    char seen[NUMBER_OF_POSSIBLE] = {0};
    for (int k = 0; k < TILES_PER_LINE; k++) {
      char v = all->subsets[s][k]->value;
      if (v == 0)
        continue;
      if (seen[v - 1]) {
        free_all_subsets(all);
        free(all);
        return 0;
      }
      seen[v - 1] = 1;
    }
  }
  free_all_subsets(all);
  free(all);
  return 1;
}

char guess_value(Grid grid) {
  for (int i = 0; i < GRID_SIZE; i++) {
    if (grid[i].value != 0)
      continue;

    int count = 0;
    int first_val = -1;
    for (int d = 0; d < GRID_SIDE; d++) {
      if (grid[i].possible[d]) {
        count++;
        if (first_val == -1)
          first_val = d + 1;
      }
    }

    if (count == 2) {

      set_tile_value(&grid[i], (char)first_val, 1);
      return 1;
    }
  }
  return 0;
}

void back_play(Grid grid) {
  if (grid == NULL || history_index <= 0)
    return;

  int affectation = history_index - 1;
  while (affectation >= 0 && history[affectation].supposed == 0)
    affectation--;

  if (affectation < 0)
    return;

  SudokuTile *guessedTile = history[affectation].tile;
  unsigned char guessedValue = history[affectation].value;

  for (int index = affectation; index < history_index; index++) {
    history[index].tile->value = 0;
  }

  for (unsigned char tile = 0; tile < NUMBER_OF_TILE_IN_A_GRID; tile++) {
    if (grid[tile].value == 0) {
      for (unsigned char possible = 0; possible < NUMBER_OF_POSSIBLE;
           possible++) {
        grid[tile].possible[possible] = 1;
      }
    }
  }

  clean_grid(grid);

  if (guessedTile != NULL && guessedValue >= 1 &&
      guessedValue <= NUMBER_OF_POSSIBLE)
    guessedTile->possible[guessedValue - 1] = 0;

  history_index = affectation;
}

void deduce_until_stable(Grid grid) {
  char modified;
  do {
    modified = 0;
    modified |= clean_grid(grid);
    modified |= solve_naked_singles(grid);
    modified |= solve_hidden_singles(grid);
    modified |= clean_naked_pairs(grid);
    modified |= clean_naked_pairs(grid);
    modified |= clean_hidden_pairs(grid);
    modified |= clean_naked_triples(grid);
    modified |= clean_hidden_triples(grid);
  } while (modified);
}

char has_pending_supposition() {
  for (int i = history_index - 1; i >= 0; i--)
    if (history[i].supposed)
      return 1;
  return 0;
}

static int filled_count(Grid grid) {
  int c = 0;
  for (int i = 0; i < NUMBER_OF_TILE_IN_A_GRID; i++)
    if (grid[i].value != 0)
      c++;
  return c;
}

char solve(Grid grid) {
  if (grid == NULL)
    return 0;
  while (1) {
    deduce_until_stable(grid);

    if (!is_grid_valid(grid)) {
      if (!has_pending_supposition())
        return 0;
      back_play(grid);
      continue;
    }

    if (filled_count(grid) == NUMBER_OF_TILE_IN_A_GRID)
      return 1;

    if (!guess_value(grid))
      return 0;
  }
}