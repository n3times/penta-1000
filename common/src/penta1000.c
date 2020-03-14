#include "penta1000_internal.h"

#include <stdlib.h>
#include <string.h>

static app_t *get_current_app(p1_t *p1) {
    app_t *app = NULL;
    if (p1->current_app_type == APP_TYPE_CALC) {
        app = (app_t *)&p1->calc;
    } else if (p1->current_app_type == APP_TYPE_GAME) {
        app = (app_t *)&p1->game;
    }
    return app;
}

/** Implementation of client interface. */

p1_t *p1_new(long seed) {
    p1_t *p1 = malloc(sizeof(p1_t));
    memset(p1, 0, sizeof(p1_t));
    init_calc(p1);
    init_game(p1, seed);
    return p1;
}

void p1_release(p1_t *p1) {
    free(p1);
}

void p1_press_key(p1_t *p1, char key) {
    app_t *app = get_current_app(p1);
    if (!app) {
        p1->current_app_type = APP_TYPE_CALC;
        app = get_current_app(p1);
        // Bypass enter animation.
        app->enter(p1);
        while (app->is_animating(p1)) {
            app->advance_frame(p1);
        }
    }
    if (key == 'g') {
        if (app == (app_t *)&p1->calc) {
            p1->current_app_type = APP_TYPE_GAME;
        } else {
            p1->current_app_type = APP_TYPE_CALC;
        }
        app = get_current_app(p1);
        app->enter(p1);
    } else {
        app->press_key(p1, key);
    }
}

char *p1_get_display(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (!app) return "PENTATRONICS";
    if (app == (app_t *)&p1->calc && p1->calc.state == CALC_STATE_COMPUTE) {
        get_calc_display(&p1->calc, p1->display);
    }
    return app->get_display(p1);
}

void p1_advance_frame(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (app) app->advance_frame(p1);
}

bool p1_is_animating(p1_t *p1) {
    app_t *app = get_current_app(p1);
    return app ? app->is_animating(p1) : false;
}
