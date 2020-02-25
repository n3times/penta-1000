#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

void new_calc(calc_t **calc) {
    *calc = malloc(sizeof(calc_t));
    memset(*calc, 0, sizeof(calc_t));
}

void release_calc(calc_t *calc) {
    free(calc);
}
