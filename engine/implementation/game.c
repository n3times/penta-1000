#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void enter_game(calc_t *calc);
static void press_key_game(calc_t *calc, key_t key);
static void advance_frame_game(calc_t *calc);
static bool is_animating_game(calc_t *calc);

void new_game(calc_t *calc) {
    game_t *game = &calc->game;

    time_t t;
    srand((unsigned) time(&t));

    game->app.enter = enter_game;
    game->app.press_key = press_key_game;
    game->app.advance_frame = advance_frame_game;
    game->app.is_animating = is_animating_game;
}

static void start_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->target = 100 + rand() % 900;
    game->guess = -1;
    game->index = 0;

    game->is_number_editing = false;
    strcpy(game->number_editing, "");

    // Start init sequence.
    game->state = GAME_STATE_INIT;
    game->frame = 0;
}

static void enter_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->state = GAME_STATE_ENTER;
    game->frame = 0;
    game->is_number_editing = false;
}

static void press_key_game(calc_t *calc, key_t key) {
    game_t *game = &calc->game;

    if (game->state == GAME_STATE_ENTER || game->state == GAME_STATE_INIT) {
        return;
    }
    if (game->state == GAME_STATE_OVER) {
        if (key == KEY_CLEAR) {
            start_game(calc);
        }
        return;
    }

    // GAME_STATE_PLAY mode.

    int is_digit = KEY_0 <= key && key <= KEY_9;
    if (game->is_number_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (game->number_editing[i] == '_') break;
        }
        if (i > 0 || key > KEY_0) {
            game->number_editing[i] = '0' + key;
            if (i == 2) {
                game->is_number_editing = false;
                game->guess = atoi(game->number_editing);
            }
        }
        if (!game->is_number_editing) {
            bool over = game->index >= 10 || game->guess == game->target;
            if (over) {
                game->state = GAME_STATE_OVER;
                game->frame = 0;
                return;
            } 
        }
    } else if (game->is_number_editing && key == KEY_CLEAR) {
        strcpy(game->number_editing, "___");
    } else {
        if (key == KEY_CLEAR) {
            start_game(calc);
            return;
        }

        if (KEY_0 < key && key <= KEY_9) {
            game->is_number_editing = true;
            game->index++;
            strcpy(game->number_editing, "___");
            game->number_editing[0] = '0' + key;
        }
    }

    if (game->is_number_editing) {
        sprintf(calc->display, "%s", game->number_editing);
    } else {
        bool high = (game->guess - game->target) > 0;
        sprintf(calc->display, 
                "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
    }
}

static void advance_frame_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->frame++;

    if (game->state == GAME_STATE_ENTER) {
        // Announce game.
        if (game->frame == 1) {
            sprintf(calc->display, "");
        } else if (game->frame == 20) {
            sprintf(calc->display, "> HI-LO GAME");
        } else if (game->frame == 90) {
            start_game(calc);
        }
    } else if (game->state == GAME_STATE_INIT) {
        // "Generate" random number.
        if (game->frame == 1) {
            sprintf(calc->display, "");
        } else if (20 < game->frame && game->frame < 90) {
            sprintf(calc->display, "%03d", rand() % 1000);
        } else if (game->frame == 90) {
            sprintf(calc->display, "???");
        } else if (game->frame == 160) {
            sprintf(calc->display, "___");
            game->state = GAME_STATE_PLAY;
        }
    } else if (game->state == GAME_STATE_PLAY) {
        // No animation.
    } else if (game->state == GAME_STATE_OVER) {
        // Animate indefinitely.
        bool did_win = game->guess == game->target;
        if (game->frame == 1) {
            sprintf(calc->display, "%03d", game->guess);
        } else if (game->frame % 200 == 0 && did_win) {
            sprintf(calc->display, "%d GUESSES", game->index);
        } else if (game->frame % 100 == 0) {
            sprintf(calc->display,
                    "YOU %s %03d", did_win ? "WON" : "LOST", game->target);
        } else if (game->frame % 100 == 50) {
            sprintf(calc->display, "");
        }
    }
}

static bool is_animating_game(calc_t *calc) {
    game_t *game = &calc->game;

    return game->state != GAME_STATE_PLAY;
}
