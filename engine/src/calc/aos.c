#include "aos.h"

#include "log.h"

#include <stdio.h>
#include <string.h>

#define MAX_MANTISSA_LEN 10

static bool is_arithmetic_op(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

static void number_to_display(double x, char *display_out) {
    bool is_negative = x < 0;

    if (is_negative) x = -x;

    if (x == 0) {
        strcpy(display_out, "0");
    } else {
        bool is_big;
        bool is_small;
        char format[5];

        sprintf(display_out, "%.0f", x);
        is_big = (strlen(display_out) > MAX_MANTISSA_LEN);

        sprintf(format, "%%.%df", MAX_MANTISSA_LEN - 1);
        sprintf(display_out, format, x);
        is_small = !strcmp(display_out, "0.000000000");

        if (is_big || is_small) {
            sprintf(display_out, "%.6e", x);
        } else {
            sprintf(display_out, "%.0f", x);
            sprintf(format, "%%.%luf", MAX_MANTISSA_LEN - strlen(display_out));
            sprintf(display_out, format, x);
            if (strchr(display_out, '.')) {
                int i = (int)strlen(display_out);
                while (display_out[i - 1] == '0') {
                    display_out[i - 1] = 0;
                    i--;
                }
                if (display_out[i - 1] == '.') display_out[i - 1] = 0;
            }
        }
        if (is_negative) {
            memmove(display_out + 1, display_out, strlen(display_out) + 1);
            display_out[0] = '-';
        }
    }
}

/* AOS API */

void aos_print(calc_t *calc, char *buffer_out, int buffer_len) {
    aos_t *aos = &calc->aos;
    char *buffer = buffer_out;
    int offset = buffer_len;

    if (offset == 0) return;

    // Null terminate the buffer.
    offset--;
    buffer[offset] = 0;
    if (offset == 0) return;

    // Start from the top of the stack and keep prepending to the buffer while
    // there is room.
    for (int i = aos->stack_depth; i >= 1; i--) {
        bool is_operand = (i % 2 == 1);
        if (is_operand) {
            int index = (i - 1) / 2;
            if (aos->operands[index].has_percent) {
                offset--;
                buffer[offset] = '%';
                if (offset == 0) return;
            }
            char display[200];
            number_to_display(aos->operands[index].number, display);
            offset -= strlen(display);
            if (offset >= 0) {
                memcpy(buffer + offset, display, strlen(display));
            } else {
                memcpy(buffer, display - offset, strlen(display) + offset);
                offset = 0;
            }
            if (offset == 0) return;
        } else {
            // Operator (1 character).
            int index = (i - 2) / 2;
            offset--;
            buffer[offset] = aos->operators[index];
            if (buffer[offset] == '*') { buffer[offset] = 'X'; }
            if (offset == 0) return;
        }
    }

    if (offset > 0) memmove(buffer, buffer + offset, buffer_len - offset);
}

void aos_eval(aos_t *aos) {
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
                aos->error = ERROR_DIV_BY_ZERO;
                return;
            }
            left.number /= right.number;
        }
        if (left.number >= 9.9999995e99 || left.number <= -9.9999995e99) {
            aos->stack_depth = 0;
            aos->error = ERROR_OVERFLOW;
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

void aos_push_operator(calc_t *calc, char op) {
    aos_t *aos = &calc->aos;
    if (aos->stack_depth == 0) return;

    if (aos->stack_depth % 2 == 0) {
        int index = aos->stack_depth / 2 - 1;
        if (is_arithmetic_op(op)) {
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
        bool loggable = aos->stack_depth > 1;
        int len = 0;
        char log[2000];
        if (loggable) {
            aos_print(calc, log, 2000);
            len = (int)strlen(log);
        }
        if (loggable) {
            aos_eval(aos);
            log[len] = '=';
            log[len + 1] = 0;
            switch(aos->error) {
            case ERROR_DIV_BY_ZERO:
                strcat(log,  "DIV BY ZERO");
                break;
            case ERROR_OVERFLOW:
                strcat(log, "OVERFLOW");
                break;
            case ERROR_OUT_OF_MEM:
                strcat(log, "OUT OF MEM");
                break;
            case ERROR_NONE:
                aos_print(calc, log + len + 1, 2000 - len - 1);
                break;
            }
            log_add_entry(&calc->log, log);
        }
    } else {
        if (is_arithmetic_op(op)) {
            aos->stack_depth++;
            aos->operators[(aos->stack_depth - 1) / 2] = op;
        }
    }
}

void aos_pop(aos_t *aos) {
    if (aos->stack_depth <= 0) return;

    if (aos->stack_depth % 2 == 1
            && aos->operands[aos->stack_depth / 2].has_percent) {
        aos->operands[aos->stack_depth / 2].has_percent = false;
    } else {
        aos->stack_depth -= 1;
    }
}
