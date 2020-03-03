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
    if (calc->is_in_game) {
        return advance_game(calc);
    } else {
        return advance_comp(calc);
    }
}

long press_key(calc_t *calc, key_t key) {
    calc->is_new = 0;

    if (key == KEY_GAME) {
        if (calc->is_in_game) {
            calc->is_in_game = false;
            return 0;
        } else {
            calc->is_in_game = true;
            reset_game(calc);
            return calc->game.wait_ms;
        }
    }

    if (calc->is_in_game) {
        return press_key_game(calc, key);
    } else {
        return press_key_comp(calc, key);
    }
}
