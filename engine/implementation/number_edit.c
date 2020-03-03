#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

// 0-9, +/- can start edit
// . can only follow a digit
void edit_number(calc_t *calc, key_t key) {
    comp_t *comp = &calc->comp;

    if (key == KEY_DOT) {
        if (strchr(comp->number_editing, '.')) return;
    }
    if (KEY_0 <= key && key <= KEY_9) {
        if (!strcmp(comp->number_editing, "0")) return;
        if (!strcmp(comp->number_editing, "-0")) return;

        int number_of_digits = strlen(comp->number_editing);
        if (strchr(comp->number_editing, '.')) number_of_digits--;
        if (strchr(comp->number_editing, '-')) number_of_digits--;
        if (number_of_digits == MAX_DIGITS_NUM) return;
    }

    if (comp->aos.stack_depth == 1) comp->aos.stack_depth = 0;
    if (key == KEY_DOT) {
        char *str = ".";
        if (!comp->is_number_editing) str = "0.";
        if (!strcmp(comp->number_editing, "")) str = "0.";
        if (!strcmp(comp->number_editing, "-")) str = "0.";
        strcat(comp->number_editing, str);
    } else if (key == KEY_CHS) {
        if (comp->number_editing[0] == '-') {
            sprintf(comp->number_editing, "%s", comp->number_editing + 1);
        } else {
            memmove(comp->number_editing + 1,
                    comp->number_editing,
                    strlen(comp->number_editing));
            comp->number_editing[0] = '-';
        }
    } else {
        sprintf(comp->number_editing, "%s%i", comp->number_editing, key);
    }
    comp->is_number_editing = 1;
}

void resolve_edit_number(calc_t *calc) {
    comp_t *comp = &calc->comp;

    double result = atof(comp->number_editing);
    double *number = &comp->aos.numbers[comp->aos.stack_depth / 2];

    *number = result;
    comp->aos.stack_depth++;
    comp->is_number_editing = 0;
    memset(comp->number_editing, 0, sizeof(comp->number_editing));
}
