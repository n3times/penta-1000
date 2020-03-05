#include "engine.h"

#include "comp.h"
#include "game.h"

#include <stdbool.h>

struct calc_s {
    bool is_new;
    bool is_in_game;
    comp_t comp;
    game_t game;
    char display[25];
};
