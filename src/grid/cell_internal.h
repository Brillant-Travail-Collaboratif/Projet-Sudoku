#ifndef CELL_INTERNAL_H
#define CELL_INTERNAL_H

#include "../sudoku_types/type_history.h"
#include "cell.h"
#include <stdlib.h>

char is_value_invalid(char value);
char are_candidates_invalid(const char candidates[CANDIDATE_COUNT]);

#endif /* CELL_INTERNAL_H */
