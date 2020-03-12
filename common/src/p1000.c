#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

static app_t *get_current_app(p1000_t *p1000) {
    app_t *app =
        p1000->is_in_game ? (app_t *)&p1000->game : (app_t *)&p1000->comp;
    return app;
}

/** Implementation of engine interface. */

p1000_t *p1000_new() {
    p1000_t *p1000 = malloc(sizeof(p1000_t));
    memset(p1000, 0, sizeof(p1000_t));
    p1000->is_new = true;
    new_comp(p1000);
    init_game(p1000);
    return p1000;
}

void p1000_release(p1000_t *p1000) {
    free(p1000);
}

void p1000_press_key(p1000_t *p1000, key_t key) {
    p1000->is_new = false;

    app_t *app = get_current_app(p1000);
    if (key == KEY_GAME) {
        p1000->is_in_game = !p1000->is_in_game;
        app->enter(p1000);
        return;
    }
    app->press_key(p1000, key);
}

char *p1000_get_display(p1000_t *p1000) {
    if (p1000->is_new) return "PENTATRONICS";

    if (!p1000->is_in_game && p1000->comp.state == COMP_STATE_COMPUTE) {
        get_p1000_display(p1000, p1000->display);
    }

    app_t *app = get_current_app(p1000);
    return app->get_display(p1000);
}

void p1000_advance_frame(p1000_t *p1000) {
    app_t *app = get_current_app(p1000);
    app->advance_frame(p1000);
}

bool p1000_is_animating(p1000_t *p1000) {
    app_t *app = get_current_app(p1000);
    return app->is_animating(p1000);
}
