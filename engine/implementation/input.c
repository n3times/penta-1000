#include "engine_internal.h"

#include <string.h>

static int is_number_edit_key(state_t *state, key_t key) {
    if (KEY_0 <= key && key < KEY_CHS) return 1;
    if (key == KEY_CHS && state->is_number_editing) return 1;
    return 0;
}

void press_key(state_t *state, key_t key) {
    if (state->is_error && key != KEY_CLEAR) return;

    if (key == KEY_CLEAR) {
        if (state->stack_depth == 2 && state->is_number_editing) {
            state->is_number_editing = 0;
        } else {
            memset(state, 0, sizeof(*state));
        }
    } else if (is_number_edit_key(state, key)) {
        edit_number(state, key);
    } else {
        if(state->is_number_editing) {
           resolve_edit_number(state);
        }
        handle_op(state, key);
    }
}
