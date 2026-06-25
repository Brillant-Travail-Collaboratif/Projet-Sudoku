# Naming Conventions

All identifiers must be written in English and use the vocabulary defined in
this document.

## Functions

Use `snake_case`. Function names must begin with a verb or verb phrase.

```c
Grid create_grid(void);
void reset_grid_candidates(Grid grid);
char solve_hidden_singles(Grid grid);
int count_filled_cells(Grid grid);
```

Avoid noun-first names and mixed casing.

```c
void grid_reset(void);                  /* noun first */
void solveHiddenSingles(Grid grid);     /* mixed casing */
```

## Variables and fields

Use `camelCase`, beginning with a lowercase letter. Names should describe the
value's role rather than its C type.

```c
int filledCount;
char *loadFile;
unsigned char candidateIndex;
SudokuCell *guessedCell;
```

Short loop indices such as `i`, `j`, `x`, and `y` are acceptable when their
meaning is obvious from a small scope. Use descriptive names for domain
objects and values that cross several statements.

## Types

Use `PascalCase` for structs, enums, unions, and typedefs. Type names should be
nouns or noun phrases and must not use suffixes such as `_t`, `Struct`, or
`DataType`.

```c
typedef struct SudokuCell {
  char value;
  char candidates[CANDIDATE_COUNT];
} SudokuCell;

typedef enum Difficulty {
  TRIVIAL,
  BASIC,
  INTERMEDIATE,
  DIFFICULT,
  EXPERT
} Difficulty;
```

## Constants and enum values

Use `SCREAMING_SNAKE_CASE`.

```c
#define GRID_SIDE 9
#define GRID_CELL_COUNT (GRID_SIDE * GRID_SIDE)
#define CANDIDATE_COUNT 9
```

## Project vocabulary

Use one term for each Sudoku concept:

| Concept | Preferred term |
| --- | --- |
| A position in the grid | `cell` |
| A value still allowed in an empty cell | `candidate` |
| A horizontal group | `row` |
| A vertical group | `column` |
| A 3x3 group | `box` |
| A row, column, or box considered generically | `subset` |
| A speculative solver assignment | `guess` |
| A solver assignment derived without guessing | `deduction` |

The CLI continues to accept both `difficile` and `difficult` for compatibility,
but internal identifiers use `DIFFICULT`.

## Checklist

- Functions use verb-first `snake_case`.
- Variables and fields use `camelCase`.
- Types use `PascalCase`.
- Constants and enum values use `SCREAMING_SNAKE_CASE`.
- Identifiers are English and use the project vocabulary.
- Names do not encode C types or pointer status.
- Public declarations and definitions use identical names.
