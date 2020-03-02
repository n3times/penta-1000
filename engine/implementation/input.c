#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static int is_number_edit_key(calc_t *calc, key_t key) {
    if (KEY_0 <= key && key < KEY_CHS) return 1;
    if (key == KEY_CHS) {
        if (calc->aos.stack_depth % 2 == 0) {
            return 1;
        }
    }
    return 0;
}

long press_key(calc_t *calc, key_t key) {
    if (key == KEY_GAME) {
        if (calc->is_game) {
            calc->is_game = 0;
        } else {
            calc->is_game = 1;
            reset_game(calc);
        }
    }

    if (calc->is_game) {
        return press_key_in_game(calc, key);
    }

    calc->is_new = 0;

    int is_error = calc->error != ERROR_NONE;
    if (is_error && key != KEY_CLEAR) return calc->wait_ms;

    if (key == KEY_CLEAR) {
        if (calc->aos.stack_depth == 2 && calc->is_number_editing) {
            calc->is_number_editing = 0;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
        } else if (calc->aos.stack_depth == 2) {
            calc->aos.stack_depth = 1;
        } else {
            memset(calc, 0, sizeof(*calc));
        }
        return calc->wait_ms;
    } else if (is_number_edit_key(calc, key)) {
        edit_number(calc, key);
        return calc->wait_ms;
    } else {
        if(calc->is_number_editing) {
           resolve_edit_number(calc);
        }
        handle_op(calc, key);
        return calc->wait_ms;
    }
}
