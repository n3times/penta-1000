#include "engine_internal.h"

static void reduce_stack(state_t *state) {
    if (state->stack_depth != 3) return;
    double result = 0;
    if (state->current_op == KEY_PLUS) {
        result = state->current_number + state->next_number;
    } else if (state->current_op == KEY_MINUS) {
        result = state->current_number - state->next_number;
    } else if (state->current_op == KEY_TIMES) {
        result = state->current_number * state->next_number;
    } else if (state->current_op == KEY_DIVIDE) {
        if (state->next_number == 0) {
            state->is_error = 1;
            return;
        }
        result = state->current_number / state->next_number;
    }
    if (result >= 1e100 || result <= -1e100) {
        state->is_error = 1;
        return;
    }
    state->current_number = result;
    state->stack_depth = 1;
}

/** Assumes, no error and no number editing */
void compute(state_t *state, key_t op) {
    if (state->stack_depth == 0) return;
    if (state->stack_depth == 2) return;

    double *number =
        state->stack_depth == 1 ? &state->current_number : &state->next_number;
    if (op == KEY_CHS) {
        *number *= -1;
    } else if (op == KEY_PERCENT) {
        *number /= 100;
    } else {
        reduce_stack(state);
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            state->current_op = op;
            state->stack_depth++;
        }
    }
}
