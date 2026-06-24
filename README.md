# Projet-Sudoku

Projet-Sudoku is a C application for generating, loading, editing, solving,
and benchmarking 9x9 Sudoku grids. It provides a command-line solver and an
interactive ncurses interface, supports reproducible puzzle generation at
several difficulty levels, and can read and write grids as text files.

## Prebuilt release

A prebuilt version of `Projet_Sudoku`, compiled on Debian 13, will be
available from the project's GitHub Releases page.

## Compile and run locally

### Requirements

The project is written in C and uses CMake 3.31.6 or newer and ncurses.

On Debian-based distribution, install the build dependencies with:

```sh
sudo apt update
sudo apt install build-essential cmake libncurses-dev
```

To run the full test suite, also install:

```sh
sudo apt install libcunit1-dev valgrind
```

### Build

From the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build --parallel
```

This creates the executable at:

```sh
build/Projet_Sudoku
```

### Run

The bundled table shortcuts are resolved relative to the CMake build
directory, so the simplest way to run the executable is:

```sh
cd build
./Projet_Sudoku -help
```

Solve one bundled table:

```sh
./Projet_Sudoku -load trivial_table_1.txt
```

Generate and solve a grid:

```sh
./Projet_Sudoku -generate basic -seed 1
```

Run the benchmark over the bundled `tables/` directory:

```sh
./Projet_Sudoku -benchmark
```

Run the interactive ncurses interface:

```sh
./Projet_Sudoku -interactive
```

### Program parameters

Both the short and long forms of each parameter are supported:

| Parameter | Description |
| --- | --- |
| `-h`, `-help` | Display the command help and exit. |
| `-l FILE`, `-load FILE` | Load and solve a grid from `FILE`. A filename without a `/` is looked up in the bundled `tables/` directory; a path containing `/` is used as provided. |
| `-g LEVEL`, `-generate LEVEL` | Generate and solve a grid. Accepted levels are `trivial`, `basic`, `intermediate`, `difficile` (or `difficult`), and `expert`. |
| `-s NUMBER`, `-seed NUMBER` | Set the generation seed, making a generated puzzle reproducible. The default seed is `1`. |
| `-i`, `-interactive` | Open the ncurses interface to edit, load, generate, solve, benchmark, and save grids interactively. |
| `-v`, `-verbose` | Print solver statistics, including the number of suppositions and deductions. |
| `-w FILE`, `-write FILE` | Write the resulting grid to `FILE` after solving or leaving the interactive interface. Empty cells are written as `?`. |
| `-b [DIR]`, `-benchmark [DIR]` | Solve every `.txt` grid in `DIR` and report timing and solver statistics. The default directory is `tables/`. |

Running the program without parameters generates and solves a `basic` grid
with seed `1`:

```sh
./Projet_Sudoku
```

Parameters can be combined where their modes are compatible. For example:

```sh
./Projet_Sudoku -generate expert -seed 42 -verbose -write solved.txt
./Projet_Sudoku -load trivial_table_1.txt -interactive
./Projet_Sudoku -benchmark ../tables -verbose
```

`-benchmark` can only be combined with `-verbose`. A loaded grid cannot also
use `-generate`, `-seed`, or `-benchmark`.

### Run tests

Configure with tests enabled, then run CTest:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Docker

The Docker image uses Alpine Linux. It builds the C executable, removes the
temporary compiler packages, and keeps the runtime/test tools, the
`Projet_Sudoku` binary, the CTest build directory, and the bundled `tables/`
directory.

Build the image from the repository root:

```sh
docker build -t projet-sudoku .
```

Run the CLI help:

```sh
docker run --rm projet-sudoku
```

Solve one bundled table:

```sh
docker run --rm projet-sudoku -load trivial_table_1.txt
```

Run the benchmark over the bundled `tables/` directory:

```sh
docker run --rm projet-sudoku -benchmark
```

Run the test suite with CTest:

```sh
docker run --rm --ulimit nofile=1024:1024 --entrypoint ctest projet-sudoku \
  --test-dir /app/build --output-on-failure
```

Run Valgrind manually:

```sh
docker run --rm --ulimit nofile=1024:1024 --entrypoint valgrind projet-sudoku \
  --leak-check=full \
  --error-exitcode=1 \
  /usr/local/bin/Projet_Sudoku -benchmark /app/tables
```

Run the interactive ncurses interface with a terminal attached:

```sh
docker run --rm -it projet-sudoku -interactive
```
