#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

// 0-9, +/- can start edit
// . can only follow a digit
void edit_number(p1000_t *p1000, char key) {
    comp_t *comp = &p1000->comp;

    if (key == '.') {
        if (strchr(comp->number_editing, '.')) return;
    }
    if ('0' <= key && key <= '9') {
        int number_of_digits = strlen(comp->number_editing);
        if (strchr(comp->number_editing, '.')) number_of_digits--;
        if (strchr(comp->number_editing, '-')) number_of_digits--;
        if (number_of_digits == MAX_DIGITS_NUM) return;
    }

    if (comp->aos.stack_depth % 2 == 1) comp->aos.stack_depth--;
    if (key == '.') {
        char *str = ".";
        if (!comp->is_number_editing) str = "0.";
        if (!strcmp(comp->number_editing, "")) str = "0.";
        if (!strcmp(comp->number_editing, "-")) str = "0.";
        strcat(comp->number_editing, str);
    } else if (key == '~') {
        if (comp->number_editing[0] == '-') {
            sprintf(comp->number_editing, "%s", comp->number_editing + 1);
        } else {
            memmove(comp->number_editing + 1,
                    comp->number_editing,
                    strlen(comp->number_editing));
            comp->number_editing[0] = '-';
        }
    } else {
        if (!strcmp(comp->number_editing, "0")) {
            strcpy(comp->number_editing, "");
        }
        if (!strcmp(comp->number_editing, "-0")) {
            strcpy(comp->number_editing, "-");
        }

        sprintf(comp->number_editing, "%s%c", comp->number_editing, key);
    }
    comp->is_number_editing = true;
}

void resolve_edit_number(p1000_t *p1000) {
    comp_t *comp = &p1000->comp;
    aos_t *aos = &comp->aos;

    double result = atof(comp->number_editing);
    double *number = &aos->operands[aos->stack_depth / 2].number;
    aos->operands[aos->stack_depth / 2].has_percent = false;

    *number = result;
    aos->stack_depth++;
    comp->is_number_editing = false;
    memset(comp->number_editing, 0, sizeof(comp->number_editing));
}
