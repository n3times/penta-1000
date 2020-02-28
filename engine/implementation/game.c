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
    game->target = rand() % 1000;

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
        calc->wait_ms = 0;
    } else if (game->animation_frame == 70) {
        sprintf(calc->display, "???");
        calc->wait_ms = 500;
    } else {
        calc->wait_ms = 10;
        sprintf(calc->display, "%03d", rand() % 1000);
    }
    return 1;
}

static void set_game_display(calc_t *calc) {
    game_t *game = &calc->game;
    char *display = calc->display;

    if (game->is_number_editing) {
        sprintf(display, " %s", game->number_editing);
    } else {
        char *score = "";
        if (game->guess == game->target) {
            score = "YOU WON ";
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

int press_key_in_game(calc_t *calc, key_t key) {
    game_t *game = &calc->game;
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
                reset_game(calc);
            }
            set_game_display(calc);
            return 1;
        }

        if (KEY_0 <= key && key <= KEY_9) {
            game->is_number_editing = 1;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }
    set_game_display(calc);
    return 1;
}
