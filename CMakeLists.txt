cmake_minimum_required(VERSION 4.2)
project(Projet_Sudoku C)

set(CMAKE_C_STANDARD 11)

include_directories(src/cli)
include_directories(src/grid)
include_directories(src/io)
include_directories(src/solver)
include_directories(src/sudoku_types)

add_executable(Projet_Sudoku
        src/cli/cli.c
        src/cli/cli.h
        src/cli/cli_internal.h
        src/grid/grid.c
        src/grid/grid.h
        src/grid/grid_internal.h
        src/io/io.c
        src/io/io.h
        src/io/io_internal.h
        src/solver/solver.c
        src/solver/solver.h
        src/solver/solver_internal.h
        src/sudoku_types/sudoku_types.h
        src/main.c)
