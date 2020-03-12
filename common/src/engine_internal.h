#include "penta1000.h"

#include "calc.h"
#include "game.h"

#include <stdbool.h>

struct p1000_s {
    app_t *current_app;
    calc_t calc;
    game_t game;
    char display[25];
};
