#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

calc_t *new_calc() {
    calc_t *calc = malloc(sizeof(calc_t));
    memset(calc, 0, sizeof(calc_t));
    calc->is_new = true;
    return calc;
}

void release_calc(calc_t *calc) {
    free(calc);
}

void advance(calc_t *calc) {
    if (calc->is_in_game) {
        advance_game(calc);
    } else {
        advance_comp(calc);
    }
}

void press_key(calc_t *calc, key_t key) {
    calc->is_new = false;

    if (key == KEY_GAME) {
        calc->is_in_game = !calc->is_in_game;
        if (calc->is_in_game) {
            appear_game(calc);
        }
        return;
    }

    if (calc->is_in_game) {
        press_key_game(calc, key);
    } else {
        press_key_comp(calc, key);
    }
}


bool is_animating(calc_t *calc) {
    if (calc->is_in_game) {
        return is_animating_game(calc);
    } else {
        return false;
    }
}
