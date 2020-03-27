#include "calc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

// Handles keys '0'-'9', '.' and '~' (sign change).
void number_edit_handle_key(calc_t *calc, char key) {
    // Make sure there is 1 dot at most.
    if (key == '.' && strchr(calc->number_editing, '.')) return;

    // Make sure there are 10 digits at most.
    if ('0' <= key && key <= '9') {
        int number_of_digits = (int)strlen(calc->number_editing);
        if (strchr(calc->number_editing, '.')) number_of_digits--;
        if (strchr(calc->number_editing, '-')) number_of_digits--;

        if (number_of_digits == MAX_DIGITS_NUM) {
            return;
        }
    }

    if (calc->aos.stack_depth % 2 == 1) {
        // Override operand if one is at the top of the stack.
        calc->aos.stack_depth--;
    }

    if (key == '.') {
        if (!calc->is_number_editing) {
            // Add '0' before '.'.
            strcpy(calc->number_editing, "0.");
        } else if (!strcmp(calc->number_editing, "-")) {
            // Add '0' before '.'.
            strcpy(calc->number_editing, "-0.");
        } else {
            // General case.
            strcat(calc->number_editing, ".");
        }
    } else if (key == '~') {
        if (calc->number_editing[0] == '-') {
            // Remove '-'.
            memmove(calc->number_editing,
                    calc->number_editing + 1,
                    strlen(calc->number_editing));
        } else {
            // Prepend '-'.
            memmove(calc->number_editing + 1,
                    calc->number_editing,
                    strlen(calc->number_editing) + 1);
            calc->number_editing[0] = '-';
        }
    } else {
        // Digit.

        // Replace leading 0.
        if (!strcmp(calc->number_editing, "0")) {
            calc->number_editing[0] = 0;
        } else if (!strcmp(calc->number_editing, "-0")) {
            calc->number_editing[1] = 0;
        }

        // Append digit.
        int len = (int)strlen(calc->number_editing);
        calc->number_editing[len] = key;
        calc->number_editing[len + 1] = 0;
    }
    calc->is_number_editing = true;
}

void number_edit_done_editing(calc_t *calc) {
    aos_t *aos = &calc->aos;

    // Place number at the top of the stack.
    if (aos->stack_depth  / 2 == 100) {
        aos->error = ERROR_OUT_OF_MEM;
        return;
    }
    operand_t *operand = &aos->operands[aos->stack_depth / 2];
    operand->number = atof(calc->number_editing);
    operand->has_percent = false;
    aos->stack_depth++;

    calc->is_number_editing = false;
    memset(calc->number_editing, 0, sizeof(calc->number_editing));
}
