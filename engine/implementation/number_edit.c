#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void edit_number(state_t *state, key_t key) {
    if (!state->is_number_editing) {
        if (key == KEY_DOT || key == KEY_CHS) {
            return;
        }
    }
    if (!strcmp(state->number_editing, "0")) {
        if (key == KEY_0) {
            return;
        }
    }

    if (state->stack_depth == 1) state->stack_depth = 0;
    if (key == KEY_DOT) {
        if (!strchr(state->number_editing, '.')) {
            strcat(state->number_editing, ".");
        }
    } else if (key == KEY_CHS) {
        if (state->number_editing[0] == '-') {
            sprintf(state->number_editing, "%s", state->number_editing + 1);
        } else {
            memmove(state->number_editing + 1,
                    state->number_editing, 
                    strlen(state->number_editing));
            state->number_editing[0] = '-';
        }
    } else {
        sprintf(state->number_editing, "%s%i", state->number_editing, key);
    }
    state->is_number_editing = 1;
}

void resolve_edit_number(state_t *state) {
    double result = atof(state->number_editing);
    double *number =
        state->stack_depth == 0 ? &state->number_1 : &state->number_2;
    *number = result;
    state->stack_depth++;
    state->is_number_editing = 0;
    memset(state->number_editing, 0, sizeof(state->number_editing));
}
