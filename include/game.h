#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "info.h"
#include "grid.h"
#include "square.h"

void start();

void init_colors();
void init_windows(WINDOW **w_grid, WINDOW **w_info);

int load_highscore_from_file();
void save_highscore_to_file(int highscore);

#endif /* GAME_H */
