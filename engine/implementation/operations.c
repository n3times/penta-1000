#include "engine_internal.h"

static void resolve_stack(calc_t *calc) {
    aos_t *aos = &calc->comp.aos;

    double result = aos->numbers[0];
    int i = 1;
    while (i + 2 <= aos->stack_depth) {
        key_t op = aos->ops[i / 2];
        double number = aos->numbers[i / 2 + 1];
        if (op == KEY_PLUS) {
            result += number;
        } else if (op == KEY_MINUS) {
            result -= number;
        } else if (op == KEY_TIMES) {
            result *= number;
        } else if (op == KEY_DIVIDE) {
            if (number == 0) {
                aos->stack_depth = 0;
                calc->comp.error = ERROR_ILLEGAL_OP;
                return;
            }
            result /= number;
        }
        if (result >= 1e100 || result <= -1e100) {
            aos->stack_depth = 0;
            calc->comp.error = ERROR_OVERFLOW;
            return;
        }
        if (-1e-100 <= result && result <= 1e-100) {
            result = 0;
        }
        i += 2;
    }
    aos->numbers[0] = result;
    aos->stack_depth = 1;
}

/** Assumes, no error and no number editing */
void handle_op(calc_t *calc, key_t op) {
    aos_t *aos = &calc->comp.aos;

    if (aos->stack_depth == 0) return;

    if (aos->stack_depth % 2 == 0) {
        int index = aos->stack_depth / 2 - 1;
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->ops[index] = op;
        }
        return;
    }

    double *number = &aos->numbers[aos->stack_depth / 2];
    if (op == KEY_CHS) {
        *number *= -1;
    } else if (op == KEY_PERCENT) {
        if (aos->stack_depth == 3) {
            if (aos->ops[0] == KEY_PLUS || aos->ops[0] == KEY_MINUS) {
                *number *= aos->numbers[0];
            }
        }
        *number /= 100;
    } else if (op == KEY_EQUAL) {
       resolve_stack(calc);
    } else {
        if (KEY_PLUS <= op && op <= KEY_DIVIDE) {
            aos->ops[aos->stack_depth / 2] = op;
            aos->stack_depth++;
        }
    }
}
