#include "engine_internal.h"

static void resolve_stack(calc_t *calc) {
    aos_t *aos = &calc->comp.aos;

    int bottom = 0;
    int top = aos->stack_depth - 1;

    while (top - bottom > 0) {
        double left = aos->operands[bottom/2].number;
        double right = aos->operands[bottom/2 + 1].number;
        key_t op = aos->operators[bottom/2];
        bool special = false;
        if (top - bottom > 3) {
            if (aos->operators[bottom/2 + 1]/2 > op/2) {
                op = aos->operators[bottom/2 + 1];
                left = aos->operands[bottom/2 + 1].number;
                right = aos->operands[bottom/2 + 2].number;
                special = true;
            }
        }
        if (op == KEY_PLUS) {
            left += right;
        } else if (op == KEY_MINUS) {
            left -= right;
        } else if (op == KEY_TIMES) {
            left *= right;
        } else if (op == KEY_DIVIDE) {
            if (right == 0) {
                aos->stack_depth = 0;
                calc->comp.error = ERROR_ILLEGAL_OP;
                return;
            }
            left /= right;
        }
        if (left >= 1e100 || left <= -1e100) {
            aos->stack_depth = 0;
            calc->comp.error = ERROR_OVERFLOW;
            return;
        }
        if (-1e-100 <= left && left <= 1e-100) {
            left = 0;
        }
        bottom += 2;
        if (special) {
            aos->operands[bottom/2 + 1].number = left;
            aos->operands[bottom/2].number = aos->operands[bottom/2 - 1].number;
            aos->operators[bottom/2] = aos->operators[bottom/2 - 1];
        } else {
            aos->operands[bottom/2].number = left;
        }
    }
    aos->operands[0].number = aos->operands[bottom/2].number;
    aos->stack_depth = 1;
}

/** Assumes, no error and no number editing */
void handle_op(calc_t *calc, key_t op) {
    aos_t *aos = &calc->comp.aos;

    if (aos->stack_depth == 0) return;

    if (aos->stack_depth % 2 == 0) {
        int index = aos->stack_depth / 2 - 1;
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->operators[index] = op;
        }
        return;
    }

    double *number = &aos->operands[aos->stack_depth / 2].number;
    if (op == KEY_CHS) {
        *number *= -1;
    } else if (op == KEY_PERCENT) {
        aos->operands[aos->stack_depth / 2].has_percent =
            !aos->operands[aos->stack_depth / 2].has_percent;
    } else if (op == KEY_EQUAL) {
       resolve_stack(calc);
    } else {
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->operators[aos->stack_depth / 2] = op;
            aos->stack_depth++;
        }
    }
}
