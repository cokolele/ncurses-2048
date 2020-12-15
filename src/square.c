#include "square.h"

int square_can_be_moved_by(int grid_levels[GRID_WIDTH][GRID_HEIGHT], int x, int y, game_keys_T dir) {
    switch (dir) {
        case UP:
            if (y == 0)
                return 0;
            else
                for (int i = 0; i < GRID_HEIGHT; i++)
                    if (y - i - 1 == -1 || grid_levels[x][y - i - 1] != 0)
                        return i;
            break;
        case RIGHT:
            if (x == GRID_WIDTH-1)
                return 0;
            else
                for (int i = 0; i < GRID_WIDTH; i++)
                    if (x + i + 1 == GRID_WIDTH || grid_levels[x + i + 1][y] != 0)
                        return i;
            break;
        case DOWN:
            if (y == GRID_HEIGHT-1)
                return 0;
            else
                for (int i = 0; i < GRID_HEIGHT; i++)
                    if (y + i + 1 == GRID_HEIGHT || grid_levels[x][y + i + 1] != 0)
                        return i;
            break;
        case LEFT:
            if (x == 0)
                return 0;
            else
                for (int i = 0; i < GRID_WIDTH; i++)
                    if (x - i - 1 == -1 || grid_levels[x - i - 1][y] != 0)
                        return i;
            break;
        default:
            break;
    }

    return 0;
}

bool square_can_be_joined(int grid_levels[GRID_WIDTH][GRID_HEIGHT], int x, int y, game_keys_T dir) {
    switch (dir) {
        case UP:
            if (y - 1 == -1 || grid_levels[x][y] != grid_levels[x][y - 1])
                return false;
            else
                return true;
        case RIGHT:
            if (x + 1 == GRID_WIDTH || grid_levels[x][y] != grid_levels[x + 1][y])
                return false;
            else
                return true;
        case DOWN:
            if (y + 1 == GRID_HEIGHT || grid_levels[x][y] != grid_levels[x][y + 1])
                return false;
            else
                return true;
        case LEFT:
            if (x - 1 == -1 || grid_levels[x][y] != grid_levels[x - 1][y])
                return false;
            else
                return true;
        default:
            return false;
    }
}