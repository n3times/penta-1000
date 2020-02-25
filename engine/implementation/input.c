#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static int is_number_edit_key(calc_t *calc, key_t key) {
    if (KEY_0 <= key && key < KEY_CHS) return 1;
    if (key == KEY_CHS) {
        if (calc->aos.stack_depth % 2 == 0) {
            return 1;
        }
    }
    return 0;
}

static void reset_game(game_t * game) {
    game->is_number_editing = 1;

    time_t t;
    srand((unsigned) time(&t));
    game->target = rand() % 1000;

    game->guess = -1;
    strcpy(game->number_editing, "___");
    game->index = 1;
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
                game->guess = atoi(game->number_editing);
            }
        } else if (key == KEY_CLEAR) {
            strcpy(game->number_editing, "___");
        }
    } else {
        if (game->index == 10 || game->guess == game->target) {
            if (key == KEY_CLEAR) {
                reset_game(game);
            }
            return;
        }

        if (KEY_0 <= key && key <= KEY_9) {
            game->is_number_editing = 1;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }
}

int press_key(calc_t *calc, key_t key) {
    if (key == KEY_GAME) {
        if (calc->is_game) {
            calc->is_game = 0;
        } else {
            calc->is_game = 1;
            reset_game(&calc->game);
        }
    }

    if (calc->is_game) {
        press_key_in_game(&calc->game, key);
        return 1;
    }

    calc->is_new = 0;

    int is_error = calc->error != ERROR_NONE;
    if (is_error && key != KEY_CLEAR) return 0;

    if (key == KEY_CLEAR) {
        if (calc->aos.stack_depth == 2 && calc->is_number_editing) {
            calc->is_number_editing = 0;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
        } else if (calc->aos.stack_depth == 2) {
            calc->aos.stack_depth = 1;
        } else {
            memset(calc, 0, sizeof(*calc));
        }
        return 1;
    } else if (is_number_edit_key(calc, key)) {
        edit_number(calc, key);
        return 1;
    } else {
        if(calc->is_number_editing) {
           resolve_edit_number(calc);
        }
        handle_op(calc, key);
        return 1;
    }
}
