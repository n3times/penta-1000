#include "engine_internal.h"

static bool is_arithmetic(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

void aos_eval(p1000_t *p1000) {
    aos_t *aos = &p1000->calc.aos;

    int bottom = 0;
    int top = aos->stack_depth - 1;

    while (top - bottom > 0) {
        operand_t left = aos->operands[bottom/2];
        operand_t right = aos->operands[bottom/2 + 1];
        char op = aos->operators[bottom/2];
        bool special = false;
        if (top - bottom > 3) {
            char op1 = op;
            char op2 = aos->operators[bottom/2 + 1];
            if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) {
                op = aos->operators[bottom/2 + 1];
                left = aos->operands[bottom/2 + 1];
                right = aos->operands[bottom/2 + 2];
                special = true;
            }
        }
        bool keep_percent = left.has_percent && right.has_percent
                && (op == '+' || op == '-');
        if (!keep_percent) {
            if (left.has_percent) left.number /= 100;
            if (right.has_percent) right.number /= 100;
        }
        if (op == '+') {
            if (!left.has_percent && right.has_percent) {
                right.number *= left.number;
            }
            left.number += right.number;
        } else if (op == '-') {
            if (!left.has_percent && right.has_percent) {
                right.number *= left.number;
            }
            left.number -= right.number;
        } else if (op == '*') {
            left.number *= right.number;
        } else if (op == '/') {
            if (right.number == 0) {
                aos->stack_depth = 0;
                p1000->calc.error = ERROR_ILLEGAL_OP;
                return;
            }
            left.number /= right.number;
        }
        if (left.number >= 1e100 || left.number <= -1e100) {
            aos->stack_depth = 0;
            p1000->calc.error = ERROR_OVERFLOW;
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

void aos_push_operator(p1000_t *p1000, char op) {
    aos_t *aos = &p1000->calc.aos;

    if (aos->stack_depth == 0) return;

    if (aos->stack_depth % 2 == 0) {
        int index = aos->stack_depth / 2 - 1;
        if (is_arithmetic(op)) {
            aos->operators[index] = op;
        }
        return;
    }

    double *number = &aos->operands[aos->stack_depth / 2].number;
    if (op == '~') {
        *number *= -1;
    } else if (op == '%') {
        aos->operands[aos->stack_depth / 2].has_percent =
            !aos->operands[aos->stack_depth / 2].has_percent;
    } else if (op == '=') {
       aos_eval(p1000);
    } else {
        if (is_arithmetic(op)) {
            aos->operators[aos->stack_depth / 2] = op;
            aos->stack_depth++;
        }
    }
}

void aos_pop(p1000_t *p1000) {
    aos_t *aos = &p1000->calc.aos;

    if (aos->stack_depth <= 0) return;

    if (aos->stack_depth % 2 == 1
            && aos->operands[aos->stack_depth / 2].has_percent) {
        aos->operands[aos->stack_depth / 2].has_percent = false;
    } else {
        aos->stack_depth -= 1;
    }
}
