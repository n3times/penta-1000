#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void x_to_d(char *formatted, double x, int len);

void get_display(state_t *state, char *display) {
    char extended_display[100];
    memset(extended_display, 0, 100);

    if (state->is_new) {
        strcpy(extended_display, "PENTATRONICS");
    } else if (state->is_data_error) {
        strcpy(extended_display, "ILLEGAL OP");
    } else if (state->is_overflow) {
        strcpy(extended_display, "OVERFLOW");
    } else {
        if (state->stack_depth >= 1) {
            char number[30];
            x_to_d(number, state->number_1, 10);
            strcat(extended_display, number);
        }
        if (state->stack_depth >= 2) {
            char *op;
            switch(state->op_1) {
                case KEY_PLUS:
                    op = "+";
                    break;
                case KEY_MINUS:
                    op = "-";
                    break;
                case KEY_TIMES:
                    op = "*";
                    break;
                case KEY_DIVIDE:
                    op = "/";
                    break;
                default:
                    op = "?";
                    break;
            }
            strcat(extended_display, op);
        }
        if (state->stack_depth == 3) {
            char number[30];
            x_to_d(number, state->number_1, 10);
            strcat(extended_display, number);
        }
        if (state->is_number_editing) {
            strcat(extended_display, state->number_editing);
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
        int is_big = 0;
        sprintf(s, "%.0f", x);
        is_big = (strlen(s) > len);

        int is_small = 0;
        sprintf(s, "%.0f", x);
        sprintf(f, "%%.%df", len - 1);
        is_small = !strcmp(s, "0.00000");

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
            memmove(s + 1, s, strlen(s));
            s[0] = '-';
        }
    }
    strcpy(formatted, result);
}
