cmake_minimum_required(VERSION 3.31.6)
project(Projet_Sudoku C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Werror -pedantic -O2 -g")
set(LDFLAGS "")

find_package(Curses REQUIRED)

set(SUDOKU_INCLUDE_DIRS
        src/file
        src/interface
        src/grid
        src/generator
        src/solver
        src/subset
        src/sudoku_types)

set(SUDOKU_SOURCES
        src/file/grid_file.c
        src/file/grid_file.h
        src/interface/benchmark.h
        src/interface/cli.c
        src/interface/cli.h
        src/interface/cli_internal.h
        src/interface/display.c
        src/interface/display.h
        src/interface/display_internal.h
        src/interface/tui.c
        src/interface/tui.h
        src/interface/tui_internal.h
        src/grid/grid.c
        src/grid/grid.h
        src/grid/grid_internal.h
        src/grid/tile.c
        src/grid/tile.h
        src/grid/tile_internal.h
        src/generator/generator.c
        src/generator/generator.h
        src/generator/generator_internal.h
        src/generator/difficulty.c
        src/generator/difficulty.h
        src/generator/random.c
        src/generator/random.h
        src/generator/random_internal.h
        src/solver/solver.c
        src/solver/solver.h
        src/solver/solver_internal.h
        src/subset/subset.c
        src/subset/subset.h
        src/subset/subset_internal.h)

add_library(sudoku_core ${SUDOKU_SOURCES})
target_include_directories(sudoku_core PUBLIC ${SUDOKU_INCLUDE_DIRS})
target_include_directories(sudoku_core PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(sudoku_core PUBLIC ${CURSES_LIBRARIES})

add_executable(Projet_Sudoku src/main.c)
target_link_libraries(Projet_Sudoku PRIVATE sudoku_core)

include(CTest)

if(BUILD_TESTING)
        add_test(NAME valgrind_test
                COMMAND valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1
                        $<TARGET_FILE:Projet_Sudoku> -benchmark ${CMAKE_SOURCE_DIR}/tables)

        find_path(CUNIT_INCLUDE_DIR CUnit/CUnit.h)
        find_library(CUNIT_LIBRARY cunit)

        if(CUNIT_INCLUDE_DIR AND CUNIT_LIBRARY)
                add_executable(sudoku_tests
                        tests/test_runner.c
                        tests/test_suites.h
                        tests/test_grid.c
                        tests/test_tile.c
                        tests/test_subset.c
                        tests/test_solver.c
                        tests/test_io.c
                        tests/test_generator.c
                        tests/test_tables.c
                        tests/test_display.c
                        tests/test_cli.c)
                target_include_directories(sudoku_tests PRIVATE ${CUNIT_INCLUDE_DIR})
                target_link_libraries(sudoku_tests PRIVATE sudoku_core ${CUNIT_LIBRARY})

                add_test(NAME cunit_tests COMMAND sudoku_tests)
        else()
                message(WARNING "CUnit was not found. Install the CUnit development package to build sudoku_tests.")
        endif()
endif()
