#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include "game.h"

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));

    if (initscr() == NULL) {
        fprintf(stderr, "Error initialising ncurses.");
        exit(EXIT_FAILURE);
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start();

    endwin();
    return EXIT_SUCCESS;
}

void start() {
    WINDOW *w_grid = NULL, *w_info = NULL;

    int grid_levels[GRID_WIDTH][GRID_HEIGHT];
    int score = 0, highscore = load_highscore_from_file();
    bool game_over = false;

    init_colors();
    init_windows(&w_grid, &w_info);
    init_grid_levels(grid_levels);

    int game_loop = 1;
    game_keys_T pressed_key = DOWN, pressed_key_prev = 9;

    while (game_loop) {
        //render
        draw_info(w_info, score, highscore, game_over);
        draw_grid(w_grid, grid_levels);
        doupdate();

        //input
        switch (getch()) {
            case 'q':
            case 'Q':
                game_loop = 0;
                break;
            case 'r':
            case 'R':
                score = 0;
                game_over = false;
                init_grid_levels(grid_levels);
                break;
            case 'w':
            case 'W':
            case 'k':
            case 'K':
            case KEY_UP:
                pressed_key = UP;
                break;
            case 'd':
            case 'D':
            case 'l':
            case 'L':
            case KEY_RIGHT:
                pressed_key = RIGHT;
                break;
            case 's':
            case 'S':
            case 'j':
            case 'J':
            case KEY_DOWN:
                pressed_key = DOWN;
                break;
            case 'a':
            case 'A':
            case 'h':
            case 'H':
            case KEY_LEFT:
                pressed_key = LEFT;
                break;
            case 10:
                draw_grid(w_grid, grid_levels);int _grid_levels[GRID_WIDTH][GRID_HEIGHT];memcpy(_grid_levels, grid_levels,sizeof (int) * GRID_WIDTH * GRID_HEIGHT);
                int p = 1, x = 0, y = 0;while (p <= 16) {if (x == GRID_WIDTH) x = 0;if (y == GRID_WIDTH) y = 0;if (p % 4)_grid_levels[x][y++] = p++;
                else _grid_levels[x++][y] = p++;}draw_grid(w_grid, _grid_levels);doupdate();getch();
                break;
            default:
                continue;
                break;
        }

        //update
        if (game_over == true)
            continue;

        update_grid(grid_levels, pressed_key, &score, &game_over);
        if (pressed_key != pressed_key_prev)
            generate_grid_level(grid_levels);

        if (score > highscore)
            highscore = score;

        //finish
        pressed_key_prev = pressed_key;
    }

    save_highscore_to_file(highscore);

    delwin(w_grid);
    delwin(w_info);
    endwin();
}

void init_colors() {
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Error initialising colors.");
        exit(EXIT_FAILURE);
    }

    start_color();
    //use_default_colors();
    assume_default_colors(COLOR_BLACK, COLOR_WHITE);

    init_pair(1, COLOR_YELLOW, COLOR_WHITE);
    init_pair(11, COLOR_BLACK, COLOR_YELLOW);

    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(22, COLOR_WHITE, COLOR_RED);

    init_pair(3, COLOR_GREEN, COLOR_WHITE);
    init_pair(33, COLOR_BLACK, COLOR_GREEN);

    init_pair(4, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(44, COLOR_WHITE, COLOR_MAGENTA);

    init_pair(5, COLOR_BLUE, COLOR_WHITE);
    init_pair(55, COLOR_WHITE, COLOR_BLUE);

    //somehow the pair index maximum is 56
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(56, COLOR_BLACK, COLOR_WHITE);
}

void init_windows(WINDOW **w_grid, WINDOW **w_info) {
    if (LINES < GRID_SIZE_V || LINES < 25) {
        endwin();
        fprintf(stderr, "Terminal is too small (height).");
        exit(EXIT_FAILURE);
    }

    if (COLS < GRID_SIZE_H || COLS < 45) {
        endwin();
        fprintf(stderr, "Terminal is too small (width).");
        exit(EXIT_FAILURE);
    }

    *w_grid = newwin(GRID_SIZE_V, GRID_SIZE_H, LINES/2 - GRID_SIZE_V/2, COLS/2 - GRID_SIZE_H/2);

    if (GRID_SIZE_H >= 45 && GRID_SIZE_V >= 25)
        *w_info = newwin(GRID_SIZE_V + 10, GRID_SIZE_H - 2, LINES/2 - GRID_SIZE_V/2 - 7, COLS/2 - GRID_SIZE_H/2 + 1);
    else
        *w_info = newwin(25 + 10, 45 - 2, LINES/2 - 25/2 - 7, COLS/2 - 45/2 + 1);

    refresh();
}

int load_highscore_from_file() {
    FILE *game_file = fopen(GAME_FILE, "r");
    int highscore_saved = 0;
    if (game_file != NULL) {
        fscanf(game_file, "%d", &highscore_saved);
        fclose(game_file);
    }

    return highscore_saved;
}

void save_highscore_to_file(int highscore) {
    FILE *game_file = fopen(GAME_FILE, "w");
    if (game_file != NULL) {
        fprintf(game_file, "%d", highscore);
        fclose(game_file);
    }
}