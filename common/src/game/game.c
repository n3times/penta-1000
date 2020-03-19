#include "penta1000_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_game(p1_t *p1);
static void press_key_game(p1_t *p1, char key);
static char *get_display_game(p1_t *p1);
static void advance_frame_game(p1_t *p1);
static bool is_animating_game(p1_t *p1);

void init_game(p1_t *p1, long seed) {
    game_t *game = &p1->game;

    game->app.enter = enter_game;
    game->app.press_key = press_key_game;
    game->app.get_display = get_display_game;
    game->app.advance_frame = advance_frame_game;
    game->app.is_animating = is_animating_game;

    game->rng = (int)(seed >= 0 ? seed : -seed);
}

static int get_random_target(p1_t *p1) {
    game_t *game = &p1->game;
    game->rng = (110351524 * game->rng + 12345) % (1 << 31);
    return (int) (game->rng % 900 + 100);
}

static void start_game(p1_t *p1) {
    game_t *game = &p1->game;

    game->target = get_random_target(p1);
    game->guess = -1;
    game->index = 0;
    game->is_guess_editing = false;

    game->state = GAME_STATE_START;
    game->frame = 0;
}

/** Implementation of the app interface. */

static void enter_game(p1_t *p1) {
    game_t *game = &p1->game;

    game->state = GAME_STATE_ENTER;
    game->frame = 0;
}

static void press_key_game(p1_t *p1, char key) {
    game_t *game = &p1->game;

    if (game->state == GAME_STATE_ENTER || game->state == GAME_STATE_START) {
        return;
    }
    if (game->state == GAME_STATE_OVER) {
        if (key == 'c') {
            start_game(p1);
        }
        return;
    }

    // GAME_STATE_PLAY mode.

    int is_digit = '0' <= key && key <= '9';

    if (!(is_digit || key == 'c')) return;

    if (game->is_guess_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (game->guess_textfield[i] == '_') break;
        }
        if (i > 0 || key != '0') {
            game->guess_textfield[i] = key;
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
    } else if (game->is_guess_editing && key == 'c') {
        strcpy(game->guess_textfield, "___");
    } else {
        if (key == 'c') {
            start_game(p1);
            return;
        }

        if ('0' < key && key <= '9') {
            game->is_guess_editing = true;
            game->index++;
            strcpy(game->guess_textfield, "___");
            game->guess_textfield[0] = key;
        } else if (key == '0') {
            return;
        }
    }

    if (game->is_guess_editing) {
        sprintf(p1->display, "%s", game->guess_textfield);
    } else {
        if (game->index == 9) {
            game->state = GAME_STATE_LAST_GUESS;
            game->frame = 0;
            sprintf(p1->display, "1 MORE GUESS");
        } else {
            bool high = (game->guess - game->target) > 0;
            sprintf(p1->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
    }
}

static char *get_display_game(p1_t *p1) {
    return p1->display;
}

static void advance_frame_game(p1_t *p1) {
    game_t *game = &p1->game;

    game->frame++;

    bool did_win = game->guess == game->target;

    switch (game->state) {
    case GAME_STATE_ENTER:
        // Announce game.
        if (game->frame == 1) {
            sprintf(p1->display, "> HI-LO GAME");
        } else if (game->frame == 80) {
            start_game(p1);
        }
        break;
    case GAME_STATE_START:
        // "Generate" random number.
        if (1 <= game->frame && game->frame < 70) {
            sprintf(p1->display, "%03d", rand() % 1000);
        } else if (game->frame == 70) {
            sprintf(p1->display, "???");
        } else if (game->frame == 140) {
            sprintf(p1->display, "___");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_PLAY:
        break;
    case GAME_STATE_LAST_GUESS:
        if (game->frame == 100) {
            bool high = (game->guess - game->target) > 0;
            sprintf(p1->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
        break;
    case GAME_STATE_OVER:
        // Animate indefinitely.
        if (game->frame == 1) {
            sprintf(p1->display, "%03d", game->guess);
        } else if (game->frame % 200 == 0 && did_win) {
            sprintf(p1->display, "%d GUESSES", game->index);
        } else if (game->frame % 100 == 0) {
            sprintf(p1->display,
                    "YOU %s %03d", did_win ? "WON" : "LOST", game->target);
        } else if (game->frame % 100 == 50) {
            sprintf(p1->display, "");
        }
        break;
    }
}

static bool is_animating_game(p1_t *p1) {
    game_t *game = &p1->game;
    return game->state != GAME_STATE_PLAY;
}
