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
    strcpy(game->number_editing, "");
    game->index = 1;

    game->frame = 0;
    game->is_animating = true;
    game->state = GAME_STATE_STARTING;
}

void advance_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->frame++;
    if (game->state == GAME_STATE_STARTING) {
        if (game->frame == 1) {
            sprintf(calc->display, "");
        } else if (game->frame == 20) {
            sprintf(calc->display, "HI-LO GAME");
        } else if (game->frame == 100) {
            sprintf(calc->display, "");
        } else if (120 < game->frame && game->frame < 190) {
            sprintf(calc->display, "%03d", rand() % 1000);
        } else if (game->frame == 190) {
            sprintf(calc->display, "???");
        } else if (game->frame == 260) {
            sprintf(calc->display, "___");
            game->is_animating = false;
            game->is_number_editing = 0;
            game->state = GAME_STATE_PLAYING;
        }
    } else if (game->state == GAME_STATE_OVER) {
        bool did_win = game->guess == game->target;
        if (game->frame == 1 || game->frame % 100 == 0) {
            sprintf(calc->display,
                    "YOU %s %03d", did_win ? "WON" : "LOST", game->target);
        } else if (game->frame % 100 == 50) {
            sprintf(calc->display, "");
        }
    }
}

static void set_game_display(calc_t *calc) {
    game_t *game = &calc->game;
    char *display = calc->display;

    if (game->is_number_editing) {
        sprintf(display, " %s", game->number_editing);
    } else {
        char *score = "";
        if (game->guess == game->target) {
            game->state = GAME_STATE_OVER;
            game->is_animating = true;
            game->frame = 0;
            return;
        } else if (game->index >= 10) {
            game->state = GAME_STATE_OVER;
            game->is_animating = true;
            game->frame = 0;
            return;
        } else {
            int delta = game->guess - game->target;
            if (delta > 0) {
                score = "TOO HIGH";
            } else {
                score = "TOO LOW";
            }
        }
        sprintf(display, "%s %03d", score, game->guess);
    }
}

void press_key_game(calc_t *calc, key_t key) {
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
        }
    } else if (game->is_number_editing && key == KEY_CLEAR) {
        strcpy(game->number_editing, "___");
    } else {
        if (key == KEY_CLEAR) {
            reset_game(calc);
            set_game_display(calc);
            return;
        }

        if (game->state == GAME_STATE_PLAYING && KEY_0 < key && key <= KEY_9) {
            game->is_number_editing = 1;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }
    set_game_display(calc);
}
