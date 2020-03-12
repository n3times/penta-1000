#include "penta1000.h"

#include "calc.h"
#include "game.h"

#include <stdbool.h>

struct p1000_s {
    bool is_new;
    bool is_in_game;
    calc_t calc;
    game_t game;
    char display[25];
};
