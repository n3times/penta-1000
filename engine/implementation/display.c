#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

static void x_to_d(char *formatted, double x, int len);
static void get_game_display(calc_t *calc, char *display);

void get_display(calc_t *calc, char *display) {
    aos_t *aos = &calc->aos;

    if (calc->is_game) {
        get_game_display(calc, display);
        return;
    }

    char extended_display[100];
    memset(extended_display, 0, 100);

    if (calc->is_new) {
        strcpy(extended_display, "PENTATRONICS");
    } else if (calc->is_data_error) {
        strcpy(extended_display, "ILLEGAL OP");
    } else if (calc->is_overflow) {
        strcpy(extended_display, "OVERFLOW");
    } else {
        if (aos->stack_depth >= 1) {
            char number[30];
            x_to_d(number, aos->number_1, MAX_DIGITS_NUM);
            strcat(extended_display, number);
        }
        if (aos->stack_depth >= 2) {
            char *op;
            switch(aos->op_1) {
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
        if (aos->stack_depth == 3) {
            printf("===>\n");
            char number[30];
            x_to_d(number, aos->number_1, MAX_DIGITS_NUM);
            strcat(extended_display, number);
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
        int is_big = 0;
        sprintf(s, "%.0f", x);
        is_big = (strlen(s) > len);

        int is_small = 0;
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

static void get_game_display(calc_t *calc, char *display) {
    game_t *game = &calc->game;

    if (game->is_number_editing) {
	if (game->index == 10) {
            sprintf(display, "GUESS %d  %s", game->index, game->number_editing); 
        } else {
            sprintf(display, "GUESS %d   %s", game->index, game->number_editing); 
        }
    } else {
        char *score = "";
        if (game->guess == game->target) {
            score = "YOU WON  ";
        } else if (game->index >= 10) {
            score = "YOU LOST ";
            sprintf(display, "%s %d", score, game->target);
            return; 
        } else {
            int delta = game->guess - game->target;
            if (delta > 0) {
                score = "TOO HIGH ";
            } else {
                score = "TOO LOW  ";
            }
        }
        sprintf(display, "%s %d", score, game->guess); 
    }
}
