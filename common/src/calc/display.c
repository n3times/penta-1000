#include "display.h"

#include "aos.h"

#include <string.h>

void update_display(calc_t *calc) {
    char extended_display[40];
    memset(extended_display, 0, 40);
    aos_t *aos = &calc->aos;

    if (aos->error == ERROR_ILLEGAL_OP) {
        strcpy(extended_display, "DIV BY ZERO");
    } else if (aos->error == ERROR_OVERFLOW) {
        strcpy(extended_display, "OVERFLOW");
    } else if (aos->error == ERROR_OUT_OF_MEM) {
        strcpy(extended_display, "OUT OF MEM");
    } else {
        aos_print(calc, extended_display, 20);
        if (calc->is_number_editing) {
            strcat(extended_display, calc->number_editing);
        }
    }
    if (extended_display[0] == '\0') {
        strcpy(extended_display, "READY");
    }
    int offset = strlen(extended_display) - 12;
    if (strchr(extended_display, '.')) offset -= 1;
    if (offset < 0) {
        offset = 0;
    }
    strcpy(calc->display, extended_display + offset);
}


