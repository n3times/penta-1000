#ifndef P1000_INTERNAL_H
#define P1000_INTERNAL_H

#include "penta1000.h"

#include "calc.h"
#include "game.h"

#include <stdbool.h>

#define PENTA_1000_VERSION 1

typedef enum apps_type_e {
    APP_TYPE_NONE = 0,
    APP_TYPE_CALC = 1,
    APP_TYPE_GAME = 2,
} app_type_t;

struct p1_s {
    long version;
    app_type_t current_app_type;
    calc_t calc;
    game_t game;
};

#endif /* P1000_INTERNAL_H */
