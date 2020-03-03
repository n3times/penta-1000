#include "engine.h"

#include "comp.h"
#include "game.h"

struct calc_s {
    int is_new;
    int is_game;
    game_t game;
    comp_t comp;
    char display[25];
};
