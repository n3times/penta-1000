#include "penta1000_internal.h"

#include "log.h"

#include <stdlib.h>
#include <string.h>

static app_t *get_current_app(p1_t *p1) {
    app_t *app = NULL;
    if (p1->current_app_type == APP_TYPE_CALC) {
        app = (app_t *)&p1->calc;
    } else if (p1->current_app_type == APP_TYPE_HILO) {
        app = (app_t *)&p1->hilo;
    } else if (p1->current_app_type == APP_TYPE_HILO2) {
        app = (app_t *)&p1->hilo2;
    }
    return app;
}

/* Implementation of the client interface. */

/* Core */

p1_t *p1_new(long seed) {
    p1_t *p1 = malloc(sizeof(p1_t));
    p1->version = PENTA_1000_VERSION;
    p1->current_app_type = APP_TYPE_NONE;
    init_calc(&p1->calc);
    init_hilo(&p1->hilo, seed);
    init_hilo2(&p1->hilo2, seed);
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
        app->enter(app);
        // Skip animation.
        while (app->is_animating(app)) {
            app->advance_frame(app);
        }
    }
    if (key == 'g' && app != (app_t *)&p1->hilo) {
        p1->current_app_type = APP_TYPE_HILO;
        app = get_current_app(p1);
        app->enter(app);
    } else if (key == 'h' && app != (app_t *)&p1->hilo2) {
        p1->current_app_type = APP_TYPE_HILO2;
        app = get_current_app(p1);
        app->enter(app);
    } else if (key == 'c' && app != (app_t *)&p1->calc) {
        p1->current_app_type = APP_TYPE_CALC;
        app = get_current_app(p1);
        app->enter(app);
    } else {
        app->press_key(app, key);
    }
}

char *p1_get_display(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (!app) return "PENTATRONICS";
    return app->get_display(app);
}

/* Animation */

void p1_advance_frame(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (app) app->advance_frame(app);
}

bool p1_is_animating(p1_t *p1) {
    app_t *app = get_current_app(p1);
    return app ? app->is_animating(app) : false;
}

/* State */

long p1_get_state_buffer_size(p1_t *p1) {
    return sizeof(p1_t);
}

char *p1_get_state_buffer(p1_t *p1) {
    long size = sizeof(p1_t);
    void *state = malloc(size);
    memcpy(state, p1, size);
    return (char *)state;
}

void p1_release_state_buffer(char *state_buffer) {
    free(state_buffer);
}

p1_t *p1_new_from_state_buffer(const char *state_buffer) {
    long version = ((p1_t *)state_buffer)->version;

    if (version != PENTA_1000_VERSION) {
        return NULL;
    }

    long struct_size = sizeof(p1_t);
    p1_t *p1 = malloc(struct_size);
    memcpy(p1, state_buffer, struct_size);
    init_calc_from_state((char *)&p1->calc);
    init_hilo_from_state((char *)&p1->hilo);
    init_hilo2_from_state((char *)&p1->hilo2);
    return p1;
}

/* Logging */

long p1_log_get_first_available_index(p1_t *p1) {
    log_t *log = &p1->calc.log;
    long first;
    log_get_available_interval(log, &first, NULL);
    return first;
}

long p1_log_get_last_available_index(p1_t *p1) {
    log_t *log = &p1->calc.log;
    long last;
    log_get_available_interval(log, NULL, &last);
    return last;
}

char *p1_log_get_entry(p1_t *p1, long index) {
    log_t *log = &p1->calc.log;
    return log_get_entry(log, index);
}

void p1_log_clear(p1_t *p1) {
    log_t *log = &p1->calc.log;
    log_clear(log);
}
