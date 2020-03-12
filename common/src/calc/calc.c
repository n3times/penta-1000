#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_calc(p1000_t *p1000);
static void press_key_calc(p1000_t *p1000, char key);
static char *get_display_calc(p1000_t *p1000);
static void advance_frame_calc(p1000_t *p1000);
static bool is_animating_calc(p1000_t *p1000);

static bool is_number_edit_key(p1000_t *p1000, char key) {
    calc_t *calc = &p1000->calc;

    if ('0' <= key && key <= '9') return true;
    if (key == '.') return true;
    if (key == '~') {
        if (calc->aos.stack_depth % 2 == 0) {
            return true;
        }
    }
    return false;
}

void new_calc(p1000_t *p1000) {
    calc_t *calc = &p1000->calc;

    calc->app.enter = enter_calc;
    calc->app.press_key = press_key_calc;
    calc->app.get_display = get_display_calc;
    calc->app.advance_frame = advance_frame_calc;
    calc->app.is_animating = is_animating_calc;

    calc->state = CALC_STATE_COMPUTE;
}

/*
 *  App interface.
 */

static void enter_calc(p1000_t *p1000) {
    calc_t *calc = &p1000->calc;
    calc->state = CALC_STATE_ENTER;
    calc->frame = 0;
}

static void press_key_calc(p1000_t *p1000, char key) {
    calc_t *calc = &p1000->calc;
    aos_t *aos = &calc->aos;
    bool is_error = calc->error != ERROR_NONE;

    if (calc->state == CALC_STATE_ENTER) return;
    if (is_error && key != 'c') return;

    if (key == 'c') {
        if (is_error) {
            aos->stack_depth = 0;
            calc->error = ERROR_NONE;
        } else if (calc->is_number_editing) {
            calc->is_number_editing = false;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
        } else {
            aos_pop(p1000);
        }
    } else if (is_number_edit_key(p1000, key)) {
        bool is_error = calc->error != ERROR_NONE;
        if (!is_error) edit_number(p1000, key);
    } else {
        bool is_error = calc->error != ERROR_NONE;
        if (!is_error) {
            if(calc->is_number_editing) {
               resolve_edit_number(p1000);
            }
            aos_push_operator(p1000, key);
        }
    }
}

static char *get_display_calc(p1000_t *p1000) {
    return p1000->display;
}

static void advance_frame_calc(p1000_t *p1000) {
    calc_t *calc = &p1000->calc;

    calc->frame++;

    switch (calc->state) {
    case CALC_STATE_ENTER:
        if (calc->frame == 1) {
            sprintf(p1000->display, ">  CALC MODE");
        } else if (calc->frame == 80) {
            calc->state = CALC_STATE_COMPUTE;
        }
        break;
    case CALC_STATE_COMPUTE:
        break;
    }
}

static bool is_animating_calc(p1000_t *p1000) {
    calc_t *calc = &p1000->calc;
    return calc->state == CALC_STATE_ENTER;
}
