#include "engine_internal.h"

#include <stdio.h>
#include <string.h>

void get_display(state_t *state, char *display) {
   char extended_display[100];
   memset(extended_display, 0, 100);

   if (state->is_error) {
	strcpy(extended_display, "ERROR");
   } else {
        if (state->stack_depth >= 1) {
            char number[30];
            sprintf(number, "%lli", (long long)state->number_1); 
            strcat(extended_display, number);
        }
        if (state->stack_depth >= 2) {
            char *op;
            switch(state->op_1) {
                case KEY_PLUS:
                    op = " + ";
                    break;
                case KEY_MINUS:
                    op = " - ";
                    break;
                case KEY_TIMES:
                    op = " * ";
                    break;
                case KEY_DIVIDE:
                    op = " / ";
                    break;
                default:
                    op = " ? ";
                    break;
            }
            strcat(extended_display, op);
        }
        if (state->stack_depth == 3) {
            char number[30];
            sprintf(number, "%lli", (long long)state->number_2); 
            strcat(extended_display, number);
        } else if (state->is_number_editing) {
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
