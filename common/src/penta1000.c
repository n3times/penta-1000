#include "p1000_internal.h"

#include <stdlib.h>
#include <string.h>

static app_t *get_current_app(p1000_t *p1000) {
    app_t *app = NULL;
    if (p1000->current_app_type == APP_TYPE_CALC) {
        app = (app_t *)&p1000->calc;
    } else if (p1000->current_app_type == APP_TYPE_GAME) {
        app = (app_t *)&p1000->game;
    }
    return app;
}

/** Implementation of client interface. */

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
    app_t *app = get_current_app(p1000);
    if (!app) {
        p1000->current_app_type = APP_TYPE_CALC;
        app = get_current_app(p1000);
        // Bypass enter animation.
        app->enter(p1000);
        while (app->is_animating(p1000)) {
            app->advance_frame(p1000);
        }
    }
    if (key == 'g') {
        if (app == (app_t *)&p1000->calc) {
            p1000->current_app_type = APP_TYPE_GAME;
        } else {
            p1000->current_app_type = APP_TYPE_CALC;
        }
        app = get_current_app(p1000);
        app->enter(p1000);
    } else {
        app->press_key(p1000, key);
    }
}

char *p1000_get_display(p1000_t *p1000) {
    app_t *app = get_current_app(p1000);
    if (!app) return "PENTATRONICS";
    if (app == (app_t *)&p1000->calc &&
            p1000->calc.state == CALC_STATE_COMPUTE) {
        get_p1000_display(p1000, p1000->display);
    }
    return app->get_display(p1000);
}

void p1000_advance_frame(p1000_t *p1000) {
    app_t *app = get_current_app(p1000);
    if (app) app->advance_frame(p1000);
}

bool p1000_is_animating(p1000_t *p1000) {
    app_t *app = get_current_app(p1000);
    if (app) return app->is_animating(p1000);
    else return false;
}
