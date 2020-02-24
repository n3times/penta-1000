#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

static int is_number_edit_key(state_t *state, key_t key) {
    if (KEY_0 <= key && key < KEY_CHS) return 1;
    if (key == KEY_CHS) {
        if (state->stack_depth % 2 == 0) {
            return 1;
        }
    }
    return 0;
}

void press_key_in_game(game_t *game, key_t key) {
    if (game->is_number_editing) {
        if (KEY_0 <= key && key <= KEY_9) {
            if (game->number_editing[0] == '_') {
                game->number_editing[0] = '0' + key;
            } else if (game->number_editing[1] == '_') {
                game->number_editing[1] = '0' + key;
            } else if (game->number_editing[2] == '_') {
                game->number_editing[2] = '0' + key;
                game->is_number_editing = 0;
                game->previous_guess = game->guess;
                game->guess = atoi(game->number_editing);
            }
        }
    } else {
        if (KEY_0 <= key && key <= KEY_9) {
            game->is_number_editing = 1;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }
}

int press_key(state_t *state, key_t key) {
    if (state->is_game) {
        press_key_in_game(&state->game, key);
        return 1;
    }

    state->is_new = 0;

    int is_error = state->is_data_error || state->is_overflow;
    if (is_error && key != KEY_CLEAR) return 0;

    if (key == KEY_CLEAR) {
        if (state->stack_depth == 2 && state->is_number_editing) {
            state->is_number_editing = 0;
            memset(state->number_editing, 0, sizeof(state->number_editing));
        } else if (state->stack_depth == 2) {
            state->stack_depth = 1;
        } else {
            memset(state, 0, sizeof(*state));
        }
        return 1;
    } else if (is_number_edit_key(state, key)) {
        edit_number(state, key);
        return 1;
    } else {
        if(state->is_number_editing) {
           resolve_edit_number(state);
        }
        handle_op(state, key);
        return 1;
    }
}
