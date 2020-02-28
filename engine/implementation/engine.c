#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

calc_t *new_calc() {
    calc_t *calc = malloc(sizeof(calc_t));
    memset(calc, 0, sizeof(calc_t));
    calc->is_new = 1;
    return calc;
}

void release_calc(calc_t *calc) {
    free(calc);
}

long get_ms_wait_to_advance(calc_t *calc) {
    return calc->wait_ms;
}

int advance(calc_t *calc) {
    if (calc->is_game) {
        advance_game(calc);
    } else {
        advance_calc(calc);
    }
    return 1;
}
