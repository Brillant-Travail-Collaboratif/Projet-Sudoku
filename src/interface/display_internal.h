#ifndef DISPLAY_INTERNAL_H
#define DISPLAY_INTERNAL_H

#include "display.h"
#include <ncurses.h>
#include <stdio.h>

#define ROW_SEPARATOR_FINAL "+-------+-------+-------+\n"
#define ROW_SEPARATOR_CANDIDATES "+-------------+-------------+-------------+\n"

void display_values_curses(Grid grid);

#endif /* DISPLAY_INTERNAL_H */
