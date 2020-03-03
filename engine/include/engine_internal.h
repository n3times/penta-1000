#include "engine.h"

#include "comp.h"
#include "game.h"

#include <stdbool.h>

struct calc_s {
    int is_new;
    bool is_in_game;
    game_t game;
    comp_t comp;
    char display[25];
};
