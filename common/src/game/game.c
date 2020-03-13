#include "p1000_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void enter_game(p1000_t *p1000);
static void press_key_game(p1000_t *p1000, char key);
static char *get_display_game(p1000_t *p1000);
static void advance_frame_game(p1000_t *p1000);
static bool is_animating_game(p1000_t *p1000);

void init_game(p1000_t *p1000) {
    game_t *game = &p1000->game;

    time_t t;
    srand((unsigned) time(&t));

    game->app.enter = enter_game;
    game->app.press_key = press_key_game;
    game->app.get_display = get_display_game;
    game->app.advance_frame = advance_frame_game;
    game->app.is_animating = is_animating_game;
}

static void start_game(p1000_t *p1000) {
    game_t *game = &p1000->game;

    game->target = 100 + rand() % 900;
    game->guess = -1;
    game->index = 0;
    game->is_guess_editing = false;

    game->state = GAME_STATE_INIT;
    game->frame = 0;
}

static void enter_game(p1000_t *p1000) {
    game_t *game = &p1000->game;

    game->state = GAME_STATE_ENTER;
    game->frame = 0;
}

static void press_key_game(p1000_t *p1000, char key) {
    game_t *game = &p1000->game;

    if (game->state == GAME_STATE_ENTER || game->state == GAME_STATE_INIT) {
        return;
    }
    if (game->state == GAME_STATE_OVER) {
        if (key == 'c') {
            start_game(p1000);
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
            start_game(p1000);
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
        sprintf(p1000->display, "%s", game->guess_textfield);
    } else {
        if (game->index == 9) {
            game->state = GAME_STATE_LAST_GUESS;
            game->frame = 0;
            sprintf(p1000->display, "1 MORE GUESS");
        } else {
            bool high = (game->guess - game->target) > 0;
            sprintf(p1000->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
    }
}

static char *get_display_game(p1000_t *p1000) {
    return p1000->display;
}

static void advance_frame_game(p1000_t *p1000) {
    game_t *game = &p1000->game;

    game->frame++;

    bool did_win = game->guess == game->target;

    switch (game->state) {
    case GAME_STATE_ENTER:
        // Announce game.
        if (game->frame == 1) {
            sprintf(p1000->display, "> HI-LO GAME");
        } else if (game->frame == 80) {
            start_game(p1000);
        }
        break;
    case GAME_STATE_INIT:
        // "Generate" random number.
        if (1 <= game->frame && game->frame < 70) {
            sprintf(p1000->display, "%03d", rand() % 1000);
        } else if (game->frame == 70) {
            sprintf(p1000->display, "???");
        } else if (game->frame == 140) {
            sprintf(p1000->display, "___");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_PLAY:
        break;
    case GAME_STATE_LAST_GUESS:
        if (game->frame == 100) {
            bool high = (game->guess - game->target) > 0;
            sprintf(p1000->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
        break;
    case GAME_STATE_OVER:
        // Animate indefinitely.
        if (game->frame == 1) {
            sprintf(p1000->display, "%03d", game->guess);
        } else if (game->frame % 200 == 0 && did_win) {
            sprintf(p1000->display, "%d GUESSES", game->index);
        } else if (game->frame % 100 == 0) {
            sprintf(p1000->display,
                    "YOU %s %03d", did_win ? "WON" : "LOST", game->target);
        } else if (game->frame % 100 == 50) {
            sprintf(p1000->display, "");
        }
        break;
    }
}

static bool is_animating_game(p1000_t *p1000) {
    game_t *game = &p1000->game;
    return game->state != GAME_STATE_PLAY;
}
