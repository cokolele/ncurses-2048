#ifndef SQUARE_H
#define SQUARE_H

#include "common.h"
#include "grid.h"

#define SQUARE_SIZE 10

#define SQUARE_DRAW_CONTENT_CHAR_LVL_1 "░"
#define SQUARE_DRAW_CONTENT_CHAR_LVL_2 "▓"
#define SQUARE_DRAW_CONTENT_CHAR_LVL_3 "█"

int square_can_be_moved_by(int grid_levels[GRID_WIDTH][GRID_HEIGHT], int x, int y, game_keys_T dir);
bool square_can_be_joined(int grid_levels[GRID_WIDTH][GRID_HEIGHT], int x, int y, game_keys_T dir);

#endif /* SQUARE_H */
