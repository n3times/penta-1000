#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_comp(p1000_t *p1000);
static void press_key_comp(p1000_t *p1000, key_t key);
static char *get_display_comp(p1000_t *p1000);
static void advance_frame_comp(p1000_t *p1000);
static bool is_animating_comp(p1000_t *p1000);

static bool is_number_edit_key(p1000_t *p1000, key_t key) {
    comp_t *comp = &p1000->comp;

    if (KEY_0 <= key && key < KEY_CHS) return true;
    if (key == KEY_CHS) {
        if (comp->aos.stack_depth % 2 == 0) {
            return true;
        }
    }
    return false;
}

void new_comp(p1000_t *p1000) {
    comp_t *comp = &p1000->comp;

    comp->app.enter = enter_comp;
    comp->app.press_key = press_key_comp;
    comp->app.get_display = get_display_comp;
    comp->app.advance_frame = advance_frame_comp;
    comp->app.is_animating = is_animating_comp;

    comp->state = COMP_STATE_COMPUTE;
}

/*
 *  App interface.
 */

static void enter_comp(p1000_t *p1000) {
    comp_t *comp = &p1000->comp;
    comp->state = COMP_STATE_ENTER;
    comp->frame = 0;
}

static void press_key_comp(p1000_t *p1000, key_t key) {
    comp_t *comp = &p1000->comp;
    aos_t *aos = &comp->aos;
    bool is_error = comp->error != ERROR_NONE;

    if (comp->state == COMP_STATE_ENTER) return;
    if (is_error && key != KEY_CLEAR) return;

    if (key == KEY_CLEAR) {
        if (is_error) {
            aos->stack_depth = 0;
            comp->error = ERROR_NONE;
        } else if (comp->is_number_editing) {
            comp->is_number_editing = false;
            memset(comp->number_editing, 0, sizeof(comp->number_editing));
        } else {
            aos_pop(p1000);
        }
    } else if (is_number_edit_key(p1000, key)) {
        bool is_error = comp->error != ERROR_NONE;
        if (!is_error) edit_number(p1000, key);
    } else {
        bool is_error = comp->error != ERROR_NONE;
        if (!is_error) {
            if(comp->is_number_editing) {
               resolve_edit_number(p1000);
            }
            aos_push_operator(p1000, key);
        }
    }
}

static char *get_display_comp(p1000_t *p1000) {
    return p1000->display;
}

static void advance_frame_comp(p1000_t *p1000) {
    comp_t *comp = &p1000->comp;

    comp->frame++;

    switch (comp->state) {
    case COMP_STATE_ENTER:
        if (comp->frame == 1) {
            sprintf(p1000->display, ">  CALC MODE");
        } else if (comp->frame == 80) {
            comp->state = COMP_STATE_COMPUTE;
        }
        break;
    case COMP_STATE_COMPUTE:
        break;
    }
}

static bool is_animating_comp(p1000_t *p1000) {
    comp_t *comp = &p1000->comp;
    return comp->state == COMP_STATE_ENTER;
}
