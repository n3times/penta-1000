#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char GAME_KEY = 'g';

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

void init_game_from_state(char *state) {
    setup_app_methods((game_t *)state);
}

static int get_random_target(game_t *game) {
    game->rng = (1103515245 * game->rng + 12345) % (1 << 31);
    return (int) (game->rng % 900 + 100);
}

static void start_game(game_t *game) {
    game->target = get_random_target(game);
    game->guess = 0;
    game->index = 0;
    game->is_guess_editing = false;

    game->state = GAME_STATE_START;
    game->frame = 0;
}

/** Implementation of the app interface. */

static void enter_game(app_t *app) {
    game_t *game = (game_t *)app;
    sprintf(game->display, "> HI-LO GAME");
    game->state = GAME_STATE_ENTER;
    game->frame = 0;
}

static void press_key_game(app_t *app, char key) {
    game_t *game = (game_t *)app;

    if (game->state == GAME_STATE_ENTER ||
        game->state == GAME_STATE_START ||
        game->state == GAME_STATE_FLASH ||
        game->state == GAME_STATE_LAST_GUESS ||
        game->state == GAME_STATE_SHOW_GUESS) {
        return;
    }
    if (game->state == GAME_STATE_OVER) {
        if (key == GAME_KEY) {
            start_game(game);
        }
        return;
    }

    // GAME_STATE_PLAY mode.

    int is_digit = '0' <= key && key <= '9';
    bool ignore_key = key != GAME_KEY && !is_digit;
    if (key == '0' && !(game->is_guess_editing && game->guess_textfield[0] != '_')) {
        ignore_key = true;
    }
    if (ignore_key) {
        strcpy(game->display, "");
        game->state = GAME_STATE_FLASH;
        game->frame = 0;
        return;
    }

    if (key == GAME_KEY) {
        if (game->is_guess_editing && game->guess_textfield[1] != '_') {
            game->guess_textfield[1] = '_';
        } else if (game->is_guess_editing && game->guess_textfield[0] != '_') {
            game->guess_textfield[0] = '_';
        } else {
            start_game(game);
            return;
        }
    } else if (game->is_guess_editing && is_digit) {
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
    } else {
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
        sprintf(game->display, "%s       ", game->guess_textfield);
    } else {
        sprintf(game->display, "%s       ", game->guess_textfield);
        game->state = GAME_STATE_SHOW_GUESS;
        game->frame = 0;
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
        if (game->frame == 80) {
            start_game(game);
        }
        break;
    case GAME_STATE_START:
        // "Generate" random number.
        if (1 <= game->frame && game->frame < 70) {
            sprintf(game->display, "%03d       ", rand() % 1000);
        } else if (game->frame == 70) {
            sprintf(game->display, "???       ");
        } else if (game->frame == 140) {
            sprintf(game->display, "___       ");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_PLAY:
        break;
    case GAME_STATE_FLASH:
        if (game->frame == 5) {
            if (game->is_guess_editing) {
                sprintf(game->display, "%s       ", game->guess_textfield);
            } else if (game->guess == 0) {
                sprintf(game->display, "___       ");
            } else {
                bool high = (game->guess - game->target) > 0;
                sprintf(game->display, "%03d %s",
                        game->guess, high ? "TOO HI" : "TOO LO");
            }
            game->state = GAME_STATE_PLAY;
            game->frame = 0;
        }
        break;
    case GAME_STATE_SHOW_GUESS:
        if (game->frame == 25) {
            if (game->index == 9) {
                game->state = GAME_STATE_LAST_GUESS;
                game->frame = 0;
                sprintf(game->display, "1 MORE GUESS");
            } else {
                bool high = (game->guess - game->target) > 0;
                sprintf(game->display, "%03d %s",
                        game->guess, high ? "TOO HI" : "TOO LO");
                game->state = GAME_STATE_PLAY;
            }
        }
        break;
    case GAME_STATE_LAST_GUESS:
        if (game->frame == 100) {
            bool high = (game->guess - game->target) > 0;
            sprintf(game->display,
                    "%03d %s", game->guess, high ? "TOO HI" : "TOO LO");
            game->state = GAME_STATE_PLAY;
        }
        break;
    case GAME_STATE_OVER:
        // Animate indefinitely.
        if (game->frame == 1) {
            sprintf(game->display, "%03d       ", game->guess);
        } else if (game->frame % 200 == 0 && did_win) {
            char *str = "GUESSES";
            if (game->index == 1) str = "GUESS";
            sprintf(game->display, "%d %s", game->index, str);
        } else if (game->frame % 100 == 0) {
            sprintf(game->display,
                    "%03d %s", game->target, did_win ? "YOU WON" : "YOU LOST");
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
