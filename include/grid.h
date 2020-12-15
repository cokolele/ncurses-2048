#ifndef GRID_H
#define GRID_H

#include "common.h"

#define GRID_WIDTH 4
#define GRID_HEIGHT 4

#include "square.h"

#define GRID_SIZE_H (SQUARE_SIZE * GRID_WIDTH + GRID_WIDTH + 1)
#define GRID_SIZE_V (SQUARE_SIZE/2 * GRID_HEIGHT + GRID_HEIGHT + 1)

void init_grid_levels(int grid_levels[GRID_WIDTH][GRID_HEIGHT]);
void generate_grid_level(int grid_levels[GRID_WIDTH][GRID_HEIGHT]);

void draw_grid(WINDOW *w_grid, int grid_levels[GRID_WIDTH][GRID_HEIGHT]);
void draw_grid_borders(WINDOW *w_grid);
void draw_grid_content(WINDOW *w_grid, int grid_levels[GRID_WIDTH][GRID_HEIGHT]);

void update_grid(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key, int *score, bool *game_over);
void move_square_if_possible(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key);
int join_square_if_possible(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key);

#endif /* GRID_H */