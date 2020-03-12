#include "engine_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void enter_game(calc_t *calc);
static void press_key_game(calc_t *calc, key_t key);
static void advance_frame_game(calc_t *calc);
static bool is_animating_game(calc_t *calc);

void init_game(calc_t *calc) {
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
    game->is_guess_editing = false;

    game->state = GAME_STATE_INIT;
    game->frame = 0;
}

static void enter_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->state = GAME_STATE_ENTER;
    game->frame = 0;
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

    if (!(is_digit || key == KEY_CLEAR)) return;

    if (game->is_guess_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (game->guess_textfield[i] == '_') break;
        }
        if (i > 0 || key > KEY_0) {
            game->guess_textfield[i] = '0' + key;
            if (i == 2) {
                game->is_guess_editing = false;
                game->guess = atoi(game->guess_textfield);
            }
        }
        if (!game->is_guess_editing) {
            bool over = game->index >= 10 || game->guess == game->target;
            if (over) {
                game->state = GAME_STATE_OVER;
                game->frame = 0;
                return;
            }
        }
    } else if (game->is_guess_editing && key == KEY_CLEAR) {
        strcpy(game->guess_textfield, "___");
    } else {
        if (key == KEY_CLEAR) {
            start_game(calc);
            return;
        }

        if (KEY_0 < key && key <= KEY_9) {
            game->is_guess_editing = true;
            game->index++;
            strcpy(game->guess_textfield, "___");
            game->guess_textfield[0] = '0' + key;
        } else if (key == KEY_0) {
            return;
        }
    }

    if (game->is_guess_editing) {
        sprintf(calc->display, "%s", game->guess_textfield);
    } else {
        if (game->index == 9) {
            game->state = GAME_STATE_LAST_GUESS;
            game->frame = 0;
            sprintf(calc->display, "1 MORE GUESS");
        } else {
            bool high = (game->guess - game->target) > 0;
            sprintf(calc->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
    }
}

static void advance_frame_game(calc_t *calc) {
    game_t *game = &calc->game;

    game->frame++;

    bool did_win = game->guess == game->target;

    switch (game->state) {
    case GAME_STATE_ENTER:
        // Announce game.
        if (game->frame == 1) {
            sprintf(calc->display, "> HI-LO GAME");
        } else if (game->frame == 80) {
            start_game(calc);
        }
        break;
    case GAME_STATE_INIT:
        // "Generate" random number.
        if (1 <= game->frame && game->frame < 70) {
            sprintf(calc->display, "%03d", rand() % 1000);
        } else if (game->frame == 70) {
            sprintf(calc->display, "???");
        } else if (game->frame == 140) {
            sprintf(calc->display, "___");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_PLAY:
        break;
    case GAME_STATE_LAST_GUESS:
        if (game->frame == 100) {
            bool high = (game->guess - game->target) > 0;
            sprintf(calc->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
        break;
    case GAME_STATE_OVER:
        // Animate indefinitely.
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
        break;
    }
}

static bool is_animating_game(calc_t *calc) {
    game_t *game = &calc->game;
    return game->state != GAME_STATE_PLAY;
}
