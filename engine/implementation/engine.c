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

long advance(calc_t *calc) {
    if (calc->is_game) {
        return advance_game(calc);
    } else {
        return advance_calc(calc);
    }
}
