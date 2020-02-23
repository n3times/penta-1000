#include "engine_internal.h"

static void reduce_stack(state_t *state) {
    if (state->stack_depth != 3) return;
    double result = 0;
    if (state->op_1 == KEY_PLUS) {
        result = state->number_1 + state->number_2;
    } else if (state->op_1 == KEY_MINUS) {
        result = state->number_1 - state->number_2;
    } else if (state->op_1 == KEY_TIMES) {
        result = state->number_1 * state->number_2;
    } else if (state->op_1 == KEY_DIVIDE) {
        if (state->number_2 == 0) {
            state->is_data_error = 1;
            return;
        }
        result = state->number_1 / state->number_2;
    }
    if (result >= 1e100 || result <= -1e100) {
        state->is_overflow = 1;
        return;
    }
    state->number_1 = result;
    state->stack_depth = 1;
}

/** Assumes, no error and no number editing */
void handle_op(state_t *state, key_t op) {
    if (state->stack_depth == 0) return;
    if (state->stack_depth == 2) return;
    if (state->stack_depth == 4) return;

    double *number =
        state->stack_depth == 1 ? &state->number_1 : &state->number_2;
    if (op == KEY_CHS) {
        *number *= -1;
    } else if (op == KEY_PERCENT) {
        if (state->stack_depth == 3) {
            if (state->op_1 == KEY_PLUS || state->op_1 == KEY_MINUS) {
                *number *= state->number_1;
            }
        }
        *number /= 100;
    } else if (state->stack_depth == 1) {
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            state->op_1 = op;
            state->stack_depth++;
        }
    } else if (state->stack_depth == 3) {
        reduce_stack(state);
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            state->op_1 = op;
            state->stack_depth++;
        }
    }
}
