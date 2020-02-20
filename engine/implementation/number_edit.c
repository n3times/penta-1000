#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void edit_number(state_t *state, key_t key) {
    sprintf(state->number_editing, "%s%i", state->number_editing, key);
    state->is_number_editing = 1;
}

void resolve_edit_number(state_t *state) {
    double result = atoll(state->number_editing);
    double *number =
        state->stack_depth == 0 ? &state->current_number : &state->next_number;
    *number = result;
    state->stack_depth++;
    state->is_number_editing = 0;
    memset(state->number_editing, 0, sizeof(state->number_editing));
}
