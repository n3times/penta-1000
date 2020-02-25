#include "engine_internal.h"

static void reduce_stack(calc_t *calc) {
    aos_t *aos = &calc->aos;

    if (aos->stack_depth != 3) return;
    double result = 0;
    if (aos->op_1 == KEY_PLUS) {
        result = aos->number_1 + aos->number_2;
    } else if (aos->op_1 == KEY_MINUS) {
        result = aos->number_1 - aos->number_2;
    } else if (aos->op_1 == KEY_TIMES) {
        result = aos->number_1 * aos->number_2;
    } else if (aos->op_1 == KEY_DIVIDE) {
        if (aos->number_2 == 0) {
            calc->error = ERROR_ILLEGAL_OP;
            return;
        }
        result = aos->number_1 / aos->number_2;
    }
    if (result >= 1e100 || result <= -1e100) {
        calc->error = ERROR_OVERFLOW;
        return;
    }
    if (-1e-100 <= result && result <= 1e-100) {
        result = 0;
    }
    aos->number_1 = result;
    aos->stack_depth = 1;
}

/** Assumes, no error and no number editing */
void handle_op(calc_t *calc, key_t op) {
    aos_t *aos = &calc->aos;

    if (aos->stack_depth == 0) return;

    if (aos->stack_depth % 2 == 0) {
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->op_1 = op;
        }
        return;
    }

    double *number =
        aos->stack_depth == 1 ? &aos->number_1 : &aos->number_2;
    if (op == KEY_CHS) {
        *number *= -1;
    } else if (op == KEY_PERCENT) {
        if (aos->stack_depth == 3) {
            if (aos->op_1 == KEY_PLUS || aos->op_1 == KEY_MINUS) {
                *number *= aos->number_1;
            }
        }
        *number /= 100;
    } else if (aos->stack_depth == 1) {
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->op_1 = op;
            aos->stack_depth++;
        }
    } else if (aos->stack_depth == 3) {
        reduce_stack(calc);
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->op_1 = op;
            aos->stack_depth++;
        }
    }
}
