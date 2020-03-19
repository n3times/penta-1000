#include "calc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MANTISSA_LEN 10

static void number_to_display(double x, char *display_out);

void aos_print(calc_t *calc, char *print_out) {
    aos_t *aos = &calc->aos;

    print_out[0] = 0;
    for (int i = 1; i <= aos->stack_depth; i++) {
        bool is_operand = (i % 2 == 1);
        if (is_operand) {
            int index = (i - 1) / 2;
            char display[20];
            number_to_display(aos->operands[index].number, display);
            strcat(print_out, display);
            if (aos->operands[index].has_percent) {
                strcat(print_out, "%");
            }
        } else {
            int index = (i - 2) / 2;
            char op = aos->operators[index];
            char op_string[2];
            op_string[0] = op;
            op_string[1] = 0;
            strcat(print_out, op_string);
        }
    }
}

void get_calc_display(calc_t *calc, char *display) {
    char extended_display[100];
    memset(extended_display, 0, 100);
    aos_t *aos = &calc->aos;

    if (aos->error == ERROR_ILLEGAL_OP) {
        strcpy(extended_display, "DIV BY ZERO");
    } else if (aos->error == ERROR_OVERFLOW) {
        strcpy(extended_display, "OVERFLOW");
    } else if (aos->error == ERROR_OUT_OF_MEM) {
        strcpy(extended_display, "OUT OF MEM");
    } else {
        int i = 1;
        if (aos->stack_depth > 10) {
            i = aos->stack_depth - 10;
        }
        for (; i <= aos->stack_depth; i++) {
            if (i % 2 == 1) {
                int index = (i - 1) / 2;
                char display[20];
                number_to_display(aos->operands[index].number, display);
                strcat(extended_display, display);
                if (aos->operands[index].has_percent) {
                    strcat(extended_display, "%");
                }
            } else {
                int index = (i - 2) / 2;
                char op = aos->operators[index];
                char op_string[2];
                op_string[0] = op;
                op_string[1] = 0;
                strcat(extended_display, op_string);
            }
        }
        if (calc->is_number_editing) {
            strcat(extended_display, calc->number_editing);
        }
    }
    if (extended_display[0] == '\0') {
        strcpy(extended_display, "READY");
    }
    int offset = strlen(extended_display) - 14;
    if (offset < 0) {
        offset = 0;
    }
    strcpy(display, extended_display + offset);
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
                int i = strlen(display_out);
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
