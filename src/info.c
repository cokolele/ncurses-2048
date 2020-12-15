#include <math.h>
#include "info.h"

void draw_info(WINDOW *w_info, int score, int highscore, bool game_over) {
    werase(w_info);

    box(w_info, 0, 0);

    int win_fourth = GRID_SIZE_H / 4;
    int win_third = GRID_SIZE_H / 3;
    int win_height = GRID_SIZE_V;

    if (GRID_SIZE_H < 45 || GRID_SIZE_V < 25) {
        win_fourth = 45 / 4;
        win_third = 45 / 3;
        win_height = 25;
    }


    //print score
    int string_length = 0, tmp = score;
    while (tmp != 0) {
        tmp /= 10;
        string_length++;
    }

    mvwaddstr(w_info, 2, win_fourth - 2, "score");
    mvwprintw(w_info, 4, win_fourth - string_length/3, "%d", score);

    //print highscore
    string_length = 0;
    tmp = highscore;
    while (tmp != 0) {
        tmp /= 10;
        string_length++;
    }

    mvwaddstr(w_info, 2, win_fourth * 3 - 6, "highscore");
    mvwprintw(w_info, 4, win_fourth * 3 - 2 - string_length/3, "%d", highscore);

    if (game_over)
        mvwaddstr(w_info, 6, win_fourth * 2 - 5, "You lost.");

    mvwaddstr(w_info, win_height + 8, 2, "arrows - move");
    mvwaddstr(w_info, win_height + 8, win_third + 4, "r - restart");
    mvwaddstr(w_info, win_height + 8, win_third*2 + 3, "q - quit");

    wnoutrefresh(w_info);
}