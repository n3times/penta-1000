#include "p1000_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

// 0-9, +/- can start edit
// . can only follow a digit
void edit_number(p1000_t *p1000, char key) {
    calc_t *calc = &p1000->calc;

    if (key == '.') {
        if (strchr(calc->number_editing, '.')) return;
    }
    if ('0' <= key && key <= '9') {
        int number_of_digits = strlen(calc->number_editing);
        if (strchr(calc->number_editing, '.')) number_of_digits--;
        if (strchr(calc->number_editing, '-')) number_of_digits--;
        if (number_of_digits == MAX_DIGITS_NUM) return;
    }

    if (calc->aos.stack_depth % 2 == 1) calc->aos.stack_depth--;
    if (key == '.') {
        char *str = ".";
        if (!calc->is_number_editing) str = "0.";
        if (!strcmp(calc->number_editing, "")) str = "0.";
        if (!strcmp(calc->number_editing, "-")) str = "0.";
        strcat(calc->number_editing, str);
    } else if (key == '~') {
        if (calc->number_editing[0] == '-') {
            sprintf(calc->number_editing, "%s", calc->number_editing + 1);
        } else {
            memmove(calc->number_editing + 1,
                    calc->number_editing,
                    strlen(calc->number_editing));
            calc->number_editing[0] = '-';
        }
    } else {
        if (!strcmp(calc->number_editing, "0")) {
            strcpy(calc->number_editing, "");
        }
        if (!strcmp(calc->number_editing, "-0")) {
            strcpy(calc->number_editing, "-");
        }

        sprintf(calc->number_editing, "%s%c", calc->number_editing, key);
    }
    calc->is_number_editing = true;
}

void resolve_edit_number(p1000_t *p1000) {
    calc_t *calc = &p1000->calc;
    aos_t *aos = &calc->aos;

    double result = atof(calc->number_editing);
    double *number = &aos->operands[aos->stack_depth / 2].number;
    aos->operands[aos->stack_depth / 2].has_percent = false;

    *number = result;
    aos->stack_depth++;
    calc->is_number_editing = false;
    memset(calc->number_editing, 0, sizeof(calc->number_editing));
}
