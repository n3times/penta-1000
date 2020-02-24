#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

void new_state(state_t **state) {
    *state = malloc(sizeof(state_t));
    memset(*state, 0, sizeof(state_t));
    (*state)->is_new = 1;
    (*state)->is_game = 1;
    (*state)->game.is_number_editing = 1;

    time_t t;
    srand((unsigned) time(&t));
    (*state)->game.target = rand() % 1000;

    (*state)->game.guess = 557;
    strcpy((*state)->game.number_editing, "___");
    (*state)->game.index = 1;
    
}

void release_state(state_t *state) {
    free(state);
}
