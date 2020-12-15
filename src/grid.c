#include <math.h>
#include <stdlib.h>
#include "grid.h"
#include "square.h"

void init_grid_levels(int grid_levels[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; i++)
        for (int j = 0; j < GRID_HEIGHT; j++)
            grid_levels[i][j] = 0;

    float start_level_blocks_count = GRID_WIDTH * GRID_HEIGHT / 100.0 * 8;
    if (start_level_blocks_count < 2)
        start_level_blocks_count = 2;

    for (int i = 0; i < start_level_blocks_count; i++) {
        int x = rand() % GRID_WIDTH;
        int y = rand() % GRID_HEIGHT;

        if (grid_levels[x][y] == 0) {
            grid_levels[x][y] = rand() < (RAND_MAX / 9) ? 2 : 1;
        } else
            i--;
    }
}


void generate_grid_level(int grid_levels[GRID_WIDTH][GRID_HEIGHT]) {
    int to_generate = GRID_WIDTH * GRID_HEIGHT / 9;
    int empty_levels[GRID_WIDTH*GRID_HEIGHT][2];
    int empty_count = 0;

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid_levels[i][j] == 0) {
                empty_levels[empty_count][0] = i;
                empty_levels[empty_count++][1] = j;
            }
        }
    }

    for (int i = 0; i < to_generate && i < empty_count; i++) {
        int *chosen_grid = empty_levels[ rand() % empty_count ];
        int x = chosen_grid[0];
        int y = chosen_grid[1];

        grid_levels[x][y] = rand() < (RAND_MAX / 13) ? 2 : 1;
    }
}

void draw_grid(WINDOW *w_grid, int grid_levels[GRID_WIDTH][GRID_HEIGHT]) {
    werase(w_grid);

    draw_grid_borders(w_grid);
    draw_grid_content(w_grid, grid_levels);

    wnoutrefresh(w_grid);
}

void update_grid(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key, int *score, bool *game_over) {
    int move_score = 0;

    move_square_if_possible(grid_levels, pressed_key);
    move_score += join_square_if_possible(grid_levels, pressed_key);
    move_square_if_possible(grid_levels, pressed_key);

    *score += move_score;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid_levels[x][y] == 0)
                return;

            game_keys_T dirs[4] = { UP, RIGHT, DOWN, LEFT };
            for (int i = 0; i < 4; ++i) {
                if (square_can_be_moved_by(grid_levels, x, y, dirs[i]) != 0)
                    return;
                if (square_can_be_joined(grid_levels, x, y, dirs[i]))
                    return;
            }
        }
    }

    *game_over = true;
}

void draw_grid_borders(WINDOW *w_grid) {
    for (int y = 0; y < GRID_SIZE_V; y++) {
        for (int x = 0; x < GRID_SIZE_H; x++) {
            if (x % (SQUARE_SIZE+1) == 0) {
                if (y == 0) {
                    if (x == 0)
                        mvwaddstr(w_grid, y, x, "┌");
                    else if (x == GRID_SIZE_H-1)
                        mvwaddstr(w_grid, y, x, "┐");
                    else
                        mvwaddstr(w_grid, y, x, "┬");
                }
                else if (y == GRID_SIZE_V-1) {
                    if (x == 0)
                        mvwaddstr(w_grid, y, x, "└");
                    else if (x == GRID_SIZE_H-1)
                        mvwaddstr(w_grid, y, x, "┘");
                    else
                        mvwaddstr(w_grid, y, x, "┴");
                }

                if (y % (SQUARE_SIZE/2+1) != 0)
                    mvwaddstr(w_grid, y, x, "│");
                else if (x != 0 && y != 0 && x != GRID_SIZE_H-1 && y != GRID_SIZE_V-1)
                    mvwaddstr(w_grid, y, x, "┼");
            }

            if (y % (SQUARE_SIZE/2+1) == 0) {
                if (x == 0) {
                    if (y != 0 && y != GRID_SIZE_V-1)
                        mvwaddstr(w_grid, y, x, "├");
                }
                else if (x == GRID_SIZE_H-1) {
                    if (y != 0 && y != GRID_SIZE_V-1)
                        mvwaddstr(w_grid, y, x, "┤");
                }

                if (x % (SQUARE_SIZE+1) != 0)
                    mvwaddstr(w_grid, y, x, "─");
            }
        }
    }
}

void draw_grid_content(WINDOW *w_grid, int grid_levels[GRID_WIDTH][GRID_HEIGHT]) {
    for (int y = 0; y < GRID_SIZE_V; y++) {
        for (int x = 0; x < GRID_SIZE_H; x++) {
            if ((x % (SQUARE_SIZE+1) == 1) && (y % (SQUARE_SIZE/2+1) == 1)) {
                int grid_level = grid_levels[ x / (SQUARE_SIZE+1) ][ y / (SQUARE_SIZE/2+1) ];

                int grid_char_level = grid_level % 3;
                int grid_color_level_fill = (grid_level + 2) / 3;
                int grid_color_level_num = grid_color_level_fill * 10 + grid_color_level_fill;
                int grid_level_shown = grid_level == 0 ? 0 : pow(2, grid_level);

                //print grid fill
                if (grid_level != 0) {
                    wattron(w_grid, COLOR_PAIR(grid_color_level_fill));
                    for (int i = 0; i < SQUARE_SIZE/2; i++) {
                        for (int j = 0; j < SQUARE_SIZE; j++) {
                            char *grid_char;
                            switch (grid_char_level) {
                                case 1:
                                    grid_char = SQUARE_DRAW_CONTENT_CHAR_LVL_1;
                                    break;
                                case 2:
                                    grid_char = SQUARE_DRAW_CONTENT_CHAR_LVL_2;
                                    break;
                                case 0:
                                    grid_char = SQUARE_DRAW_CONTENT_CHAR_LVL_3;
                                    break;
                            }

                            mvwaddstr(w_grid, y + i, x + j, grid_char);
                        }
                    }
                    wattroff(w_grid, COLOR_PAIR(grid_color_level_fill));
                }

                //print grid number
                int grid_level_length = 0;
                int _grid_level = grid_level_shown;
                while (_grid_level != 0) {
                    _grid_level /= 10;
                    grid_level_length++;
                }
                if (grid_level == 0)
                    grid_level_length = 1;

                int num_x = x + SQUARE_SIZE/2 - grid_level_length/2;
                int num_y = y + SQUARE_SIZE/4;

                if (SQUARE_SIZE % 2 == 0 && grid_level_length % 2)
                    num_x--;

                if (grid_level == 16)
                    grid_color_level_num = 56;

                if (grid_level != 0)
                    wattron(w_grid, COLOR_PAIR(grid_color_level_num));

                if (grid_level < 16)
                    mvwprintw(w_grid, num_y, num_x, "%d", grid_level_shown);
                else {
                    num_x++;
                    mvwprintw(w_grid, num_y, num_x, "%s", "wow!");
                }

                if (grid_level != 0)
                    wattroff(w_grid, COLOR_PAIR(grid_color_level_num));
            }
        }
    }
}

void move_square_if_possible(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key) {
    switch (pressed_key) {
        case UP:
            for (int y = 1; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0) {
                        int steps = square_can_be_moved_by(grid_levels, x, y, UP);
                        if (steps) {
                            grid_levels[x][y - steps] = grid_levels[x][y];
                            grid_levels[x][y] = 0;
                        }
                    }
                }
            }
            break;
        case RIGHT:
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = GRID_WIDTH - 2; x >= 0; x--) {
                    if (grid_levels[x][y] != 0) {
                        int steps = square_can_be_moved_by(grid_levels, x, y, RIGHT);
                        if (steps) {
                            grid_levels[x + steps][y] = grid_levels[x][y];
                            grid_levels[x][y] = 0;
                        }
                    }
                }
            }
            break;
        case DOWN:
            for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0) {
                        int steps = square_can_be_moved_by(grid_levels, x, y, DOWN);
                        if (steps) {
                            grid_levels[x][y + steps] = grid_levels[x][y];
                            grid_levels[x][y] = 0;
                        }
                    }
                }
            }
            break;
        case LEFT:
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 1; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0) {
                        int steps = square_can_be_moved_by(grid_levels, x, y, LEFT);
                        if (steps) {
                            grid_levels[x - steps][y] = grid_levels[x][y];
                            grid_levels[x][y] = 0;
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}

int join_square_if_possible(int grid_levels[GRID_WIDTH][GRID_HEIGHT], game_keys_T pressed_key) {
    int move_score = 0;

    switch (pressed_key) {
        case UP:
            for (int y = 1; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0 && square_can_be_joined(grid_levels, x, y, UP)) {
                        grid_levels[x][y - 1] = grid_levels[x][y] + 1;
                        grid_levels[x][y] = 0;
                        move_score += pow(2, grid_levels[x][y - 1]);
                    }
                }
            }
            break;
        case RIGHT:
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = GRID_WIDTH - 2; x >= 0; x--) {
                    if (grid_levels[x][y] != 0 && square_can_be_joined(grid_levels, x, y, RIGHT)) {
                        grid_levels[x + 1][y] = grid_levels[x][y] + 1;
                        grid_levels[x][y] = 0;
                        move_score += pow(2, grid_levels[x + 1][y]);
                    }
                }
            }
            break;
        case DOWN:
            for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0 && square_can_be_joined(grid_levels, x, y, DOWN)) {
                        grid_levels[x][y + 1] = grid_levels[x][y] + 1;
                        grid_levels[x][y] = 0;
                        move_score += pow(2, grid_levels[x][y + 1]);
                    }
                }
            }
            break;
        case LEFT:
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 1; x < GRID_WIDTH; x++) {
                    if (grid_levels[x][y] != 0 && square_can_be_joined(grid_levels, x, y, LEFT)) {
                        grid_levels[x - 1][y] = grid_levels[x][y] + 1;
                        grid_levels[x][y] = 0;
                        move_score += pow(2, grid_levels[x - 1][y]);
                    }
                }
            }
            break;
        default:
            break;
    }

    return move_score;
}