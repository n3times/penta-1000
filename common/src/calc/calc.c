#include "penta1000_internal.h"

#include "aos.h"
#include "number_edit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_calc(p1_t *p1);
static void press_key_calc(p1_t *p1, char key);
static char *get_display_calc(p1_t *p1);
static void advance_frame_calc(p1_t *p1);
static bool is_animating_calc(p1_t *p1);

static bool is_number_edit_key(p1_t *p1, char key) {
    calc_t *calc = &p1->calc;

    if ('0' <= key && key <= '9') return true;
    if (key == '.') return true;
    if (key == '~') {
        if (calc->aos.stack_depth % 2 == 0) {
            return true;
        }
    }
    return false;
}

void init_calc(p1_t *p1) {
    calc_t *calc = &p1->calc;

    calc->app.enter = enter_calc;
    calc->app.press_key = press_key_calc;
    calc->app.get_display = get_display_calc;
    calc->app.advance_frame = advance_frame_calc;
    calc->app.is_animating = is_animating_calc;
}

/* App interface. */

static void enter_calc(p1_t *p1) {
    calc_t *calc = &p1->calc;
    calc->state = CALC_STATE_ENTER;
    calc->frame = 0;
}

static void press_key_calc(p1_t *p1, char key) {
    calc_t *calc = &p1->calc;
    aos_t *aos = &calc->aos;
    bool is_error = aos->error != ERROR_NONE;

    if (calc->state == CALC_STATE_ENTER) return;
    if (is_error && key != 'c') return;

    if (key == 'c') {
        if (is_error) {
            aos->stack_depth = 0;
            calc->is_number_editing = false;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
            aos->error = ERROR_NONE;
        } else if (calc->is_number_editing) {
            calc->is_number_editing = false;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
        } else {
            aos_pop(aos);
        }
    } else if (is_number_edit_key(p1, key)) {
        bool is_error = aos->error != ERROR_NONE;
        if (!is_error) number_edit_handle_key(calc, key);
    } else {
        bool is_error = aos->error != ERROR_NONE;
        if (!is_error) {
            if(calc->is_number_editing) {
               number_edit_done_editing(calc);
            }
            aos_push_operator(calc, key);
        }
    }
}

static char *get_display_calc(p1_t *p1) {
    return p1->display;
}

static void advance_frame_calc(p1_t *p1) {
    calc_t *calc = &p1->calc;

    calc->frame++;

    switch (calc->state) {
    case CALC_STATE_ENTER:
        if (calc->frame == 1) {
            sprintf(p1->display, ">  CALC MODE");
        } else if (calc->frame == 80) {
            calc->state = CALC_STATE_COMPUTE;
        }
        break;
    case CALC_STATE_COMPUTE:
        break;
    }
}

static bool is_animating_calc(p1_t *p1) {
    calc_t *calc = &p1->calc;
    return calc->state == CALC_STATE_ENTER;
}
