#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void enter_comp(calc_t *calc);
static void press_key_comp(calc_t *calc, key_t key);
static void advance_frame_comp(calc_t *calc);
static bool is_animating_comp(calc_t *calc);

void new_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;

    comp->app.enter = enter_comp;
    comp->app.press_key = press_key_comp;
    comp->app.advance_frame = advance_frame_comp;
    comp->app.is_animating = is_animating_comp;

    comp->state = COMP_STATE_COMPUTE;
}

static void enter_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;
    comp->frame = 0;
    comp->state = COMP_STATE_APPEAR;
}

static void advance_frame_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;

    comp->frame++;
    if (comp->state == COMP_STATE_APPEAR) {
        if (comp->frame == 1) {
            sprintf(calc->display, "");
        } else if (comp->frame == 20) {
            sprintf(calc->display, " CALCULATOR ");
            sprintf(calc->display, ">  CALC MODE");
        } else if (comp->frame == 90) {
            sprintf(calc->display, "");
        } else if (comp->frame == 110) {
            comp->state = COMP_STATE_COMPUTE;
        }
    }
}

static bool is_animating_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;
    return comp->state == COMP_STATE_APPEAR;
}

static int is_number_edit_key(calc_t *calc, key_t key) {
    comp_t *comp = &calc->comp;

    if (KEY_0 <= key && key < KEY_CHS) return 1;
    if (key == KEY_CHS) {
        if (comp->aos.stack_depth % 2 == 0) {
            return 1;
        }
    }
    return 0;
}

static void press_key_comp(calc_t *calc, key_t key) {
    comp_t *comp = &calc->comp;

    int is_error = comp->error != ERROR_NONE;
    if (is_error && key != KEY_CLEAR) return;

    if (key == KEY_CLEAR) {
        if (comp->aos.stack_depth <= 1) {
            memset(comp, 0, sizeof(*calc));
            new_comp(calc);
            new_game(calc);
        } else if (comp->is_number_editing) {
            comp->is_number_editing = 0;
            memset(comp->number_editing, 0, sizeof(comp->number_editing));
        } else {
            comp->aos.stack_depth -= 1;
        }
        return;
    } else if (is_number_edit_key(calc, key)) {
        edit_number(calc, key);
        return;
    } else {
        if(comp->is_number_editing) {
           resolve_edit_number(calc);
        }
        handle_op(calc, key);
        return;
    }
}
