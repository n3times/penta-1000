#include "penta1000.h"

#include "comp.h"
#include "game.h"

#include <stdbool.h>

struct p1000_s {
    bool is_new;
    bool is_in_game;
    comp_t comp;
    game_t game;
    char display[25];
};
