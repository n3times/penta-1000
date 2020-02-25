#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

// 0-9, +/- can start edit
// . can only follow a digit
void edit_number(calc_t *calc, key_t key) {
    if (key == KEY_DOT) {
        if (!calc->is_number_editing) return;
        if (strchr(calc->number_editing, '.')) return;
        if (!strcmp(calc->number_editing, "")) return;
        if (!strcmp(calc->number_editing, "-")) return;
    }
    if (KEY_0 <= key && key <= KEY_9) {
        if (!strcmp(calc->number_editing, "0")) return;
        if (!strcmp(calc->number_editing, "-0")) return;

        int number_of_digits = strlen(calc->number_editing);
        if (strchr(calc->number_editing, '.')) number_of_digits--;
        if (strchr(calc->number_editing, '-')) number_of_digits--;
        if (number_of_digits == MAX_DIGITS_NUM) return;
    }

    if (calc->aos.stack_depth == 1) calc->aos.stack_depth = 0;
    if (key == KEY_DOT) {
        strcat(calc->number_editing, ".");
    } else if (key == KEY_CHS) {
        if (calc->number_editing[0] == '-') {
            sprintf(calc->number_editing, "%s", calc->number_editing + 1);
        } else {
            memmove(calc->number_editing + 1,
                    calc->number_editing,
                    strlen(calc->number_editing));
            calc->number_editing[0] = '-';
        }
    } else {
        sprintf(calc->number_editing, "%s%i", calc->number_editing, key);
    }
    calc->is_number_editing = 1;
}

void resolve_edit_number(calc_t *calc) {
    double result = atof(calc->number_editing);
    double *number =
        calc->aos.stack_depth == 0 ? &calc->aos.number_1 : &calc->aos.number_2;

    *number = result;
    calc->aos.stack_depth++;
    calc->is_number_editing = 0;
    memset(calc->number_editing, 0, sizeof(calc->number_editing));
}
