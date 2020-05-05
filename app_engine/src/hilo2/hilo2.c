#include "hilo2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char GAME_KEY = 'h';

static void enter(app_t *app);
static void press_key(app_t *app, char key);
static char *get_display(app_t *app);
static void advance_frame(app_t *app);
static bool is_animating(app_t *app);

static void setup_app_methods(hilo2_t *hilo2) {
    hilo2->app.enter = enter;
    hilo2->app.press_key = press_key;
    hilo2->app.get_display = get_display;
    hilo2->app.advance_frame = advance_frame;
    hilo2->app.is_animating = is_animating;
}

void init_hilo2(hilo2_t *hilo2, long seed) {
    memset(hilo2, 0, sizeof(hilo2_t));
    setup_app_methods(hilo2);
    hilo2->rng = (int)(seed >= 0 ? seed : -seed);
}

void init_hilo2_from_state(char *state) {
    setup_app_methods((hilo2_t *)state);
}

static int get_random_target(hilo2_t *hilo2) {
    hilo2->rng = (1103515245 * hilo2->rng + 12345) % (1 << 31);
    return (int) (hilo2->rng % 900 + 100);
}

static void start_hilo2(hilo2_t *hilo2) {
    hilo2->target = get_random_target(hilo2);
    hilo2->guess = 0;
    hilo2->index = 0;
    hilo2->is_guess_editing = false;

    hilo2->state = HILO2_STATE_START;
    hilo2->frame = 0;
}

/** Implementation of the app interface. */

static void enter(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    sprintf(hilo2->display, "> HI-LO GAME");
    hilo2->state = HILO2_STATE_ENTER;
    hilo2->frame = 0;
}

static void press_key(app_t *app, char key) {
    hilo2_t *hilo2 = (hilo2_t *)app;

    if (hilo2->state == HILO2_STATE_ENTER ||
        hilo2->state == HILO2_STATE_START ||
        hilo2->state == HILO2_STATE_FLASH ||
        hilo2->state == HILO2_STATE_STATS ||
        hilo2->state == HILO2_STATE_LAST_GUESS ||
        hilo2->state == HILO2_STATE_SHOW_GUESS) {
        return;
    }
    if (hilo2->state == HILO2_STATE_OVER) {
        if (key == GAME_KEY) {
            start_hilo2(hilo2);
        }
        return;
    }

    // HILO2_STATE_PLAY mode.

    int is_digit = '0' <= key && key <= '9';
    bool ignore_key = key != GAME_KEY && key != '%' && !is_digit;
    if (key == '0' && !(hilo2->is_guess_editing && hilo2->guess_textfield[0] != '_')) {
        ignore_key = true;
    }
    if (ignore_key) {
        strcpy(hilo2->display, "");
        hilo2->state = HILO2_STATE_FLASH;
        hilo2->frame = 0;
        return;
    }

    if (key == GAME_KEY) {
        if (hilo2->is_guess_editing && hilo2->guess_textfield[1] != '_') {
            hilo2->guess_textfield[1] = '_';
        } else if (hilo2->is_guess_editing && hilo2->guess_textfield[0] != '_') {
            hilo2->guess_textfield[0] = '_';
        } else {
            start_hilo2(hilo2);
            return;
        }
    } else if (key == '%') {
        char *str = "GUESSES";
        if (hilo2->index == 1) str = "GUESS";
        sprintf(hilo2->display, "%d %s", hilo2->index, str);
        hilo2->state = HILO2_STATE_STATS;
        hilo2->frame = 0;
        return;
    } else if (hilo2->is_guess_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (hilo2->guess_textfield[i] == '_') break;
        }
        if (i > 0 || key != '0') {
            hilo2->guess_textfield[i] = key;
            if (i == 2) {
                hilo2->is_guess_editing = false;
                hilo2->guess = atoi(hilo2->guess_textfield);
                hilo2->index++;
            }
        }
        if (!hilo2->is_guess_editing) {
            bool over = hilo2->index >= 10 || hilo2->guess == hilo2->target;
            if (over) {
                hilo2->state = HILO2_STATE_OVER;
                hilo2->frame = 0;
                return;
            }
        }
    } else {
        if ('0' < key && key <= '9') {
            hilo2->is_guess_editing = true;
            strcpy(hilo2->guess_textfield, "___");
            hilo2->guess_textfield[0] = key;
        } else if (key == '0') {
            return;
        }
    }

    if (hilo2->is_guess_editing) {
        sprintf(hilo2->display, "%s       ", hilo2->guess_textfield);
    } else {
        sprintf(hilo2->display, "%s       ", hilo2->guess_textfield);
        hilo2->state = HILO2_STATE_SHOW_GUESS;
        hilo2->frame = 0;
    }
}

static char *get_display(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    return hilo2->display;
}

static void go_to_play_state(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;

    if (hilo2->is_guess_editing) {
        sprintf(hilo2->display, "%s       ", hilo2->guess_textfield);
    } else if (hilo2->guess == 0) {
        sprintf(hilo2->display, "___ GUESS ");
    } else {
        bool high = (hilo2->guess - hilo2->target) > 0;
        sprintf(hilo2->display, "%03d %s",
                hilo2->guess, high ? "TOO HI" : "TOO LO");
    }
    hilo2->state = HILO2_STATE_PLAY;
}

static void advance_frame(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;

    hilo2->frame++;

    bool did_win = hilo2->guess == hilo2->target;

    switch (hilo2->state) {
    case HILO2_STATE_ENTER:
        if (hilo2->frame == 80) {
            start_hilo2(hilo2);
        }
        break;
    case HILO2_STATE_START:
        if (1 <= hilo2->frame && hilo2->frame < 100) {
            sprintf(hilo2->display, "%d       ", get_random_target(hilo2));
        } else if (hilo2->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO2_STATE_PLAY:
        break;
    case HILO2_STATE_FLASH:
        if (hilo2->frame == 5) {
            go_to_play_state(app);
        }
        break;
    case HILO2_STATE_STATS:
        if (hilo2->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO2_STATE_SHOW_GUESS:
        if (hilo2->frame == 25) {
            if (hilo2->index == 9) {
                hilo2->state = HILO2_STATE_LAST_GUESS;
                hilo2->frame = 0;
                sprintf(hilo2->display, "1 MORE GUESS");
            } else {
                go_to_play_state(app);
            }
        }
        break;
    case HILO2_STATE_LAST_GUESS:
        if (hilo2->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO2_STATE_OVER:
        if (hilo2->frame == 1) {
            sprintf(hilo2->display, "%03d       ", hilo2->guess);
        } else if (hilo2->frame % 200 == 0 && did_win) {
            char *str = "GUESSES";
            if (hilo2->index == 1) str = "GUESS";
            sprintf(hilo2->display, "%d %s", hilo2->index, str);
        } else if (hilo2->frame % 100 == 0) {
            sprintf(hilo2->display,
                    "%03d %s", hilo2->target, did_win ? "YOU WON" : "YOU LOST");
        } else if (hilo2->frame % 100 == 50) {
            sprintf(hilo2->display, "");
        }
        break;
    }
}

static bool is_animating(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    return hilo2->state != HILO2_STATE_PLAY;
}
