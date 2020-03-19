#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_game(app_t *app);
static void press_key_game(app_t *app, char key);
static char *get_display_game(app_t *app);
static void advance_frame_game(app_t *app);
static bool is_animating_game(app_t *app);

static void setup_app_methods(game_t *game) {
    game->app.enter = enter_game;
    game->app.press_key = press_key_game;
    game->app.get_display = get_display_game;
    game->app.advance_frame = advance_frame_game;
    game->app.is_animating = is_animating_game;
}

void init_game(game_t *game, long seed) {
    memset(game, 0, sizeof(game_t));
    setup_app_methods(game);
    game->rng = (int)(seed >= 0 ? seed : -seed);
}

void deserialize_game(game_t *game) {
    setup_app_methods(game);
}

static int get_random_target(game_t *game) {
    game->rng = (110351524 * game->rng + 12345) % (1 << 31);
    return (int) (game->rng % 900 + 100);
}

static void start_game(game_t *game) {
    game->target = get_random_target(game);
    game->guess = -1;
    game->index = 0;
    game->is_guess_editing = false;

    game->state = GAME_STATE_START;
    game->frame = 0;
}

/** Implementation of the app interface. */

static void enter_game(app_t *app) {
    game_t *game = (game_t *)app;

    game->state = GAME_STATE_ENTER;
    game->frame = 0;
}

static void press_key_game(app_t *app, char key) {
    game_t *game = (game_t *)app;

    if (game->state == GAME_STATE_ENTER || game->state == GAME_STATE_START) {
        return;
    }
    if (game->state == GAME_STATE_OVER) {
        if (key == 'c') {
            start_game(game);
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
            start_game(game);
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
        sprintf(game->display, "%s", game->guess_textfield);
    } else {
        if (game->index == 9) {
            game->state = GAME_STATE_LAST_GUESS;
            game->frame = 0;
            sprintf(game->display, "1 MORE GUESS");
        } else {
            bool high = (game->guess - game->target) > 0;
            sprintf(game->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
    }
}

static char *get_display_game(app_t *app) {
    game_t *game = (game_t *)app;
    return game->display;
}

static void advance_frame_game(app_t *app) {
    game_t *game = (game_t *)app;

    game->frame++;

    bool did_win = game->guess == game->target;

    switch (game->state) {
    case GAME_STATE_ENTER:
        // Announce game.
        if (game->frame == 1) {
            sprintf(game->display, "> HI-LO GAME");
        } else if (game->frame == 80) {
            start_game(game);
        }
        break;
    case GAME_STATE_START:
        // "Generate" random number.
        if (1 <= game->frame && game->frame < 70) {
            sprintf(game->display, "%03d", rand() % 1000);
        } else if (game->frame == 70) {
            sprintf(game->display, "???");
        } else if (game->frame == 140) {
            sprintf(game->display, "___");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_PLAY:
        break;
    case GAME_STATE_LAST_GUESS:
        if (game->frame == 100) {
            bool high = (game->guess - game->target) > 0;
            sprintf(game->display,
                    "TOO %s %03d", high ? "HIGH" : "LOW", game->guess);
        }
        break;
    case GAME_STATE_OVER:
        // Animate indefinitely.
        if (game->frame == 1) {
            sprintf(game->display, "%03d", game->guess);
        } else if (game->frame % 200 == 0 && did_win) {
            sprintf(game->display, "%d GUESSES", game->index);
        } else if (game->frame % 100 == 0) {
            sprintf(game->display,
                    "YOU %s %03d", did_win ? "WON" : "LOST", game->target);
        } else if (game->frame % 100 == 50) {
            sprintf(game->display, "");
        }
        break;
    }
}

static bool is_animating_game(app_t *app) {
    game_t *game = (game_t *)app;
    return game->state != GAME_STATE_PLAY;
}
