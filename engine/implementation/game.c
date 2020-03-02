#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void reset_game(calc_t *calc) {
    game_t *game = &calc->game;
    game->is_number_editing = 1;

    time_t t;
    srand((unsigned) time(&t));
    game->target = 100 + rand() % 900;

    game->guess = -1;
    strcpy(game->number_editing, "___");
    game->index = 1;

    game->animation_frame = 0;
    calc->wait_ms = 10;
}

int advance_game(calc_t *calc) {
    game_t *game = &calc->game;
    game->animation_frame++;
    
    if (game->animation_frame == 71) {
        sprintf(calc->display, "___");
        calc->wait_ms = ADVANCE_NONE;
    } else if (game->animation_frame == 70) {
        sprintf(calc->display, "???");
        calc->wait_ms = 500;
    } else {
        calc->wait_ms = 10;
        sprintf(calc->display, "%03d", rand() % 1000);
    }
    return calc->wait_ms;
}

static void set_game_display(calc_t *calc) {
    game_t *game = &calc->game;
    char *display = calc->display;

    if (game->is_number_editing) {
        sprintf(display, " %s", game->number_editing);
    } else {
        char *score = "";
        if (game->guess == game->target) {
            score = "YOU WON";
        } else if (game->index >= 10) {
            score = "YOU LOST";
            sprintf(display, "%s %d", score, game->target);
            return;
        } else {
            int delta = game->guess - game->target;
            if (delta > 0) {
                score = "TOO HIGH";
            } else {
                score = "TOO LOW ";
            }
        }
        sprintf(display, "%s %03d", score, game->guess);
    }
}

long press_key_in_game(calc_t *calc, key_t key) {
    game_t *game = &calc->game;
    int is_digit = KEY_0 <= key && key <= KEY_9;
    if (game->is_number_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (game->number_editing[i] == '_') break;
        }
        if (i > 0 || key > KEY_0) {
            game->number_editing[i] = '0' + key;
            if (i == 2) {
                game->is_number_editing = 0;
                game->guess = atoi(game->number_editing);
            }
        } else if (key == KEY_CLEAR) {
            strcpy(game->number_editing, "___");
        }
    } else {
        if (game->index == 10 || game->guess == game->target) {
            if (key == KEY_CLEAR) {
                reset_game(calc);
            }
            set_game_display(calc);
            return calc->wait_ms;
        }

        if (KEY_0 < key && key <= KEY_9) {
            game->is_number_editing = 1;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }
    set_game_display(calc);
    return calc->wait_ms;
}
