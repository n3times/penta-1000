#include "engine_internal.h"

static void resolve_stack(calc_t *calc) {
    aos_t *aos = &calc->comp.aos;

    int bottom = 0;
    int top = aos->stack_depth - 1;

    while (top - bottom > 0) {
        operand_t left = aos->operands[bottom/2];
        operand_t right = aos->operands[bottom/2 + 1];
        key_t op = aos->operators[bottom/2];
        bool special = false;
        if (top - bottom > 3) {
            if (aos->operators[bottom/2 + 1]/2 > op/2) {
                op = aos->operators[bottom/2 + 1];
                left = aos->operands[bottom/2 + 1];
                right = aos->operands[bottom/2 + 2];
                special = true;
            }
        }
        bool keep_percent = left.has_percent && right.has_percent
                && (op == KEY_PLUS || op == KEY_MINUS);
        if (!keep_percent) {
            if (left.has_percent) left.number /= 100;
            if (right.has_percent) right.number /= 100;
        }
        if (op == KEY_PLUS) {
            if (!left.has_percent && right.has_percent) {
                right.number *= left.number;
            }
            left.number += right.number;
        } else if (op == KEY_MINUS) {
            if (!left.has_percent && right.has_percent) {
                right.number *= left.number;
            }
            left.number -= right.number;
        } else if (op == KEY_TIMES) {
            left.number *= right.number;
        } else if (op == KEY_DIVIDE) {
            if (right.number == 0) {
                aos->stack_depth = 0;
                calc->comp.error = ERROR_ILLEGAL_OP;
                return;
            }
            left.number /= right.number;
        }
        if (left.number >= 1e100 || left.number <= -1e100) {
            aos->stack_depth = 0;
            calc->comp.error = ERROR_OVERFLOW;
            return;
        }
        if (-1e-100 <= left.number && left.number <= 1e-100) {
            left.number = 0;
        }
        bottom += 2;
        if (!keep_percent) {
            left.has_percent = false;
            right.has_percent = false;
        }
        if (special) {
            aos->operands[bottom/2 + 1] = left;
            aos->operands[bottom/2] = aos->operands[bottom/2 - 1];
            aos->operators[bottom/2] = aos->operators[bottom/2 - 1];
        } else {
            aos->operands[bottom/2] = left;
        }
    }
    aos->operands[0] = aos->operands[bottom/2];
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
