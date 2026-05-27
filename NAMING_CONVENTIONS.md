# Naming Conventions

This document outlines the naming conventions for the SUDOKU project. All code must follow these rules to maintain consistency and readability.

## General Rules

- **Language**: All identifiers must be written in **English**
- **Character Case**: Follow the specific rules for each type of identifier (see sections below)
- **Type Information**: Never include type information in variable or data structure names (no prefixes or suffixes like `strName`, `namePtr`, `pName`, etc.)

---

## Functions

**Rule**: All function names must start with a **verb** and use **camelCase**

### Examples

✅ **Good**:
```c
void init_grid();
void disp_final();
void disp_possible();
void set_tile_value();
char load_grid_from_file();
```

❌ **Bad**:
```c
void SUDOKUCalculation();  // noun, not verb
void grid();                 // not descriptive
void strRead();              // contains type info
```

---

## Variables

**Rule**: Use **camelCase**, starting with a lowercase letter

### Examples

✅ **Good**:
```c
int gridSize;
char* userInput;
bool isValid;
float gridWidth;
```

❌ **Bad**:
```c
int grid_size;              // snake_case
int GRID_SIZE;              // SCREAMING_SNAKE_CASE
int strInput;               // contains type info (str)
int ptrGrid;                // contains type info (ptr)
```

---

## Constants

**Rule**: Use **SCREAMING_SNAKE_CASE** (all uppercase with underscores)

### Examples

✅ **Good**:
```c
#define MAX_GRID_SIZE 100
#define MIN_CLUE_VALUE 0
#define DEFAULT_TIMEOUT 5000
```

❌ **Bad**:
```c
#define maxGridSize 100      // camelCase
#define MAX_GRID_SIZE_INT 100  // contains type info
```

---

## Data Structures (struct, enum, union)

**Rule**: Use **camelCase** with a noun or nominal group

### Examples

✅ **Good**:
```c
struct gridCell {
    int value;
    bool isMarked;
};

enum cellState {
    EMPTY,
    FILLED,
    MARKED
};

typedef struct {
    int width;
    int height;
} gridDimension;
```

❌ **Bad**:
```c
struct grid_cell { };        // snake_case
struct GRID_CELL { };        // SCREAMING_SNAKE_CASE
struct grid_cell_t { };      // contains type info (_t)
struct gridCellStruct { };   // contains type info (Struct)
```

---

## Typedef

**Rule**: Use the same **camelCase** naming as the underlying data structure (no additional suffixes like `_t`)

### Examples

✅ **Good**:
```c
typedef struct {
    char value;
    char possible [9] 
} sudoku_tile;

typedef int sudokuValue;
typedef char* gridString;
```

❌ **Bad**:
```c
typedef struct { } coordinateT;      // contains type info (_t)
typedef int sudokuValue_t;         // contains type info (_t)
```

---

## Summary Table

| Identifier Type | Case Style | Example | Notes |
|---|---|---|---|
| Functions | camelCase (verb first) | `calculateGrid()` | Must start with action verb |
| Variables | camelCase | `gridSize` | No type information |
| Constants | SCREAMING_SNAKE_CASE | `MAX_GRID_SIZE` | All uppercase with underscores |
| Structs | camelCase (noun) | `gridCell` | No type suffix |
| Enums | camelCase (noun) | `cellState` | No type suffix |
| Typedefs | camelCase | `coordinate` | No `_t` suffix |
| Enum Values | SCREAMING_SNAKE_CASE | `CELL_FILLED` | All uppercase with underscores |

---

## Quick Checklist

Before committing code, verify:

- [ ] All identifiers are in English
- [ ] Functions use camelCase and start with a verb
- [ ] Variables use camelCase
- [ ] Constants use SCREAMING_SNAKE_CASE
- [ ] Data structures (struct/enum/union) use camelCase with nouns
- [ ] No type information is included in any identifier names
- [ ] Typedefs don't have `_t` or similar type suffixes

