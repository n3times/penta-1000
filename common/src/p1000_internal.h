#include "penta1000.h"

#include "calc.h"
#include "game.h"

#include <stdbool.h>

typedef enum apps_type_e {
    APP_TYPE_NONE = 0,
    APP_TYPE_CALC = 1,
    APP_TYPE_GAME = 2,
} app_type_t;

struct p1_s {
    app_type_t current_app_type;
    calc_t calc;
    game_t game;
    char display[25];
};
