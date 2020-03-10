#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_comp(calc_t *calc);
static void press_key_comp(calc_t *calc, key_t key);
static void advance_frame_comp(calc_t *calc);
static bool is_animating_comp(calc_t *calc);

static bool is_number_edit_key(calc_t *calc, key_t key) {
    comp_t *comp = &calc->comp;

    if (KEY_0 <= key && key < KEY_CHS) return true;
    if (key == KEY_CHS) {
        if (comp->aos.stack_depth % 2 == 0) {
            return true;
        }
    }
    return false;
}

void new_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;

    comp->app.enter = enter_comp;
    comp->app.press_key = press_key_comp;
    comp->app.advance_frame = advance_frame_comp;
    comp->app.is_animating = is_animating_comp;

    comp->state = COMP_STATE_COMPUTE;
}

/*
 *  App interface.
 */

static void enter_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;
    comp->state = COMP_STATE_ENTER;
    comp->frame = 0;
}

static void press_key_comp(calc_t *calc, key_t key) {
    comp_t *comp = &calc->comp;
    aos_t *aos = &comp->aos;

    if (comp->state == COMP_STATE_ENTER) return;

    if (key == KEY_CLEAR) {
        if (comp->is_number_editing) {
            comp->is_number_editing = false;
            memset(comp->number_editing, 0, sizeof(comp->number_editing));
        } else {
            if (aos->stack_depth % 2 == 1
                    && aos->operands[aos->stack_depth / 2].has_percent) {
                comp->aos.operands[aos->stack_depth / 2].has_percent = false;
            } else {
                comp->aos.stack_depth -= 1;
            }
        }
    } else if (is_number_edit_key(calc, key)) {
        bool is_error = comp->error != ERROR_NONE;
        if (!is_error) edit_number(calc, key);
    } else {
        bool is_error = comp->error != ERROR_NONE;
        if (!is_error) {
            if(comp->is_number_editing) {
               resolve_edit_number(calc);
            }
            handle_op(calc, key);
        }
    }
}

static void advance_frame_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;

    comp->frame++;

    switch (comp->state) {
    case COMP_STATE_ENTER:
        if (comp->frame == 1) {
            sprintf(calc->display, ">  CALC MODE");
        } else if (comp->frame == 80) {
            comp->state = COMP_STATE_COMPUTE;
        }
        break;
    case COMP_STATE_COMPUTE:
        break;
    }
}

static bool is_animating_comp(calc_t *calc) {
    comp_t *comp = &calc->comp;
    return comp->state == COMP_STATE_ENTER;
}
