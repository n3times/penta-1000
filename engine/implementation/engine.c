#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

calc_t *new_calc() {
    calc_t *calc = malloc(sizeof(calc_t));
    memset(calc, 0, sizeof(calc_t));
    calc->is_new = true;
    new_game(calc);
    new_comp(calc);
    return calc;
}

void release_calc(calc_t *calc) {
    free(calc);
}

void advance(calc_t *calc) {
    app_t *app = calc->is_in_game ? (app_t *)&calc->game : (app_t *)&calc->comp;
    app->advance_frame(calc);
}

void press_key(calc_t *calc, key_t key) {
    calc->is_new = false;

    if (key == KEY_GAME) {
        calc->is_in_game = !calc->is_in_game;
        if (calc->is_in_game) {
            calc->game.app.enter(calc);
        }
        return;
    }

    app_t *app = calc->is_in_game ? (app_t *)&calc->game : (app_t *)&calc->comp;
    app->press_key(calc, key);
}


bool is_animating(calc_t *calc) {
    app_t *app = calc->is_in_game ? (app_t *)&calc->game : (app_t *)&calc->comp;
    return app->is_animating(calc);
}
