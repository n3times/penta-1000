#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

/** Implementation of engine interface. */

p1000_t *p1000_new() {
    p1000_t *p1000 = malloc(sizeof(p1000_t));
    memset(p1000, 0, sizeof(p1000_t));
    new_calc(p1000);
    init_game(p1000);
    return p1000;
}

void p1000_release(p1000_t *p1000) {
    free(p1000);
}

void p1000_press_key(p1000_t *p1000, char key) {
    if (!p1000->current_app) {
        p1000->current_app = (app_t *)&p1000->calc;
        p1000->current_app->enter(p1000);
        while (p1000->current_app->is_animating(p1000)) {
            p1000->current_app->advance_frame(p1000);
        }
    }
    if (key == 'g') {
        if (p1000->current_app == (app_t *)&p1000->calc) {
            p1000->current_app = (app_t *)&p1000->game;
        } else {
            p1000->current_app = (app_t *)&p1000->calc;
        }
        p1000->current_app->enter(p1000);
        return;
    }
    p1000->current_app->press_key(p1000, key);
}

char *p1000_get_display(p1000_t *p1000) {
    app_t *current_app = p1000->current_app;
    if (!current_app) return "PENTATRONICS";
    if (current_app == (app_t *)&p1000->calc &&
            p1000->calc.state == CALC_STATE_COMPUTE) {
        get_p1000_display(p1000, p1000->display);
    }
    return current_app->get_display(p1000);
}

void p1000_advance_frame(p1000_t *p1000) {
    if (p1000->current_app) p1000->current_app->advance_frame(p1000);
}

bool p1000_is_animating(p1000_t *p1000) {
    if (p1000->current_app) return p1000->current_app->is_animating(p1000);
    else return false;
}
