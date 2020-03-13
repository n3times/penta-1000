#include "calc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

static void x_to_d(char *formatted, double x, int len);

void get_calc_display(calc_t *calc, char *display) {
    char extended_display[100];
    memset(extended_display, 0, 100);
    aos_t *aos = &calc->aos;

    if (aos->error == ERROR_ILLEGAL_OP) {
        strcpy(extended_display, "DIV BY ZERO");
    } else if (aos->error == ERROR_OVERFLOW) {
        strcpy(extended_display, "OVERFLOW");
    } else {
        for (int i = 1; i <= aos->stack_depth; i++) {
            if (i % 2 == 1) {
                int index = (i - 1) / 2;
                char number[30];
                x_to_d(number, aos->operands[index].number, MAX_DIGITS_NUM);
                strcat(extended_display, number);
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
    int offset = strlen(extended_display) - 12;
    if (offset < 0) {
        offset = 0;
    }
    strcpy(display, extended_display + offset);
}

static void x_to_d(char *formatted, double x, int len) {
    char s[200];
    char f[5];
    char *result = s;
    int is_negative = x < 0;

    if (is_negative) x = -x;
    if (x == 0) {
        result = "0";
    } else {
        bool is_big;
        bool is_small;

        sprintf(s, "%.0f", x);
        is_big = (strlen(s) > len);

        sprintf(f, "%%.%df", len - 1);
        sprintf(s, f, x);
        is_small = !strcmp(s, "0.000000000");

        if (is_big || is_small) {
            sprintf(s, "%.6e", x);
        } else {
            sprintf(s, "%.0f", x);
            sprintf(f, "%%.%luf", len - strlen(s));
            sprintf(s, f, x);
            if (strchr(s, '.')) {
                int i = strlen(s);
                while (s[i - 1] == '0') {
                    s[i - 1] = 0;
                    i--;
                }
                if (s[i - 1] == '.') s[i - 1] = 0;
            }
        }
        if (is_negative) {
            memmove(s + 1, s, strlen(s) + 1);
            s[0] = '-';
        }
    }
    strcpy(formatted, result);
}
