#include "hilo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char GAME_KEY = 'g';

static void enter(app_t *app);
static void press_key(app_t *app, char key);
static char *get_display(app_t *app);
static void advance_frame(app_t *app);
static bool is_animating(app_t *app);

static void setup_app_methods(hilo_t *hilo) {
    hilo->app.enter = enter;
    hilo->app.press_key = press_key;
    hilo->app.get_display = get_display;
    hilo->app.advance_frame = advance_frame;
    hilo->app.is_animating = is_animating;
}

void init_hilo(hilo_t *hilo, long seed) {
    memset(hilo, 0, sizeof(hilo_t));
    setup_app_methods(hilo);
    hilo->rng = (int)(seed >= 0 ? seed : -seed);
}

void init_hilo_from_state(char *state) {
    setup_app_methods((hilo_t *)state);
}

static int get_random_target(hilo_t *hilo) {
    hilo->rng = (1103515245 * hilo->rng + 12345) % (1 << 31);
    return (int) (hilo->rng % 900 + 100);
}

static void start_hilo(hilo_t *hilo) {
    hilo->target = get_random_target(hilo);
    hilo->guess = 0;
    hilo->index = 0;
    hilo->is_guess_editing = false;

    hilo->state = HILO_STATE_START;
    hilo->frame = 0;
}

/** Implementation of the app interface. */

static void enter(app_t *app) {
    hilo_t *hilo = (hilo_t *)app;
    sprintf(hilo->display, "> HI-LO GAME");
    hilo->state = HILO_STATE_ENTER;
    hilo->frame = 0;
}

static void press_key(app_t *app, char key) {
    hilo_t *hilo = (hilo_t *)app;

    if (hilo->state == HILO_STATE_ENTER ||
        hilo->state == HILO_STATE_START ||
        hilo->state == HILO_STATE_FLASH ||
        hilo->state == HILO_STATE_STATS ||
        hilo->state == HILO_STATE_LAST_GUESS ||
        hilo->state == HILO_STATE_SHOW_GUESS) {
        return;
    }
    if (hilo->state == HILO_STATE_OVER) {
        if (key == GAME_KEY) {
            start_hilo(hilo);
        }
        return;
    }

    // HILO_STATE_PLAY mode.

    int is_digit = '0' <= key && key <= '9';
    bool ignore_key = key != GAME_KEY && key != '%' && !is_digit;
    if (key == '0' && !(hilo->is_guess_editing && hilo->guess_textfield[0] != '_')) {
        ignore_key = true;
    }
    if (ignore_key) {
        strcpy(hilo->display, "");
        hilo->state = HILO_STATE_FLASH;
        hilo->frame = 0;
        return;
    }

    if (key == GAME_KEY) {
        if (hilo->is_guess_editing && hilo->guess_textfield[1] != '_') {
            hilo->guess_textfield[1] = '_';
        } else if (hilo->is_guess_editing && hilo->guess_textfield[0] != '_') {
            hilo->guess_textfield[0] = '_';
        } else {
            start_hilo(hilo);
            return;
        }
    } else if (key == '%') {
        char *str = "GUESSES";
        if (hilo->index == 1) str = "GUESS";
        sprintf(hilo->display, "%d %s", hilo->index, str);
        hilo->state = HILO_STATE_STATS;
        hilo->frame = 0;
        return;
    } else if (hilo->is_guess_editing && is_digit) {
        int i = 0;
        for (i = 0; i < 3; i++) {
            if (hilo->guess_textfield[i] == '_') break;
        }
        if (i > 0 || key != '0') {
            hilo->guess_textfield[i] = key;
            if (i == 2) {
                hilo->is_guess_editing = false;
                hilo->guess = atoi(hilo->guess_textfield);
                hilo->index++;
            }
        }
        if (!hilo->is_guess_editing) {
            bool over = hilo->index >= 10 || hilo->guess == hilo->target;
            if (over) {
                hilo->state = HILO_STATE_OVER;
                hilo->frame = 0;
                return;
            }
        }
    } else {
        if ('0' < key && key <= '9') {
            hilo->is_guess_editing = true;
            strcpy(hilo->guess_textfield, "___");
            hilo->guess_textfield[0] = key;
        } else if (key == '0') {
            return;
        }
    }

    if (hilo->is_guess_editing) {
        sprintf(hilo->display, "%s       ", hilo->guess_textfield);
    } else {
        sprintf(hilo->display, "%s       ", hilo->guess_textfield);
        hilo->state = HILO_STATE_SHOW_GUESS;
        hilo->frame = 0;
    }
}

static char *get_display(app_t *app) {
    hilo_t *hilo = (hilo_t *)app;
    return hilo->display;
}

static void go_to_play_state(app_t *app) {
    hilo_t *hilo = (hilo_t *)app;

    if (hilo->is_guess_editing) {
        sprintf(hilo->display, "%s       ", hilo->guess_textfield);
    } else if (hilo->guess == 0) {
        sprintf(hilo->display, "___ GUESS ");
    } else {
        bool high = (hilo->guess - hilo->target) > 0;
        sprintf(hilo->display, "%03d %s",
                hilo->guess, high ? "TOO HI" : "TOO LO");
    }
    hilo->state = HILO_STATE_PLAY;
    hilo->frame = -1;
}

static void advance_frame(app_t *app) {
    hilo_t *hilo = (hilo_t *)app;

    if (hilo->frame < 0) return;

    hilo->frame++;

    bool did_win = hilo->guess == hilo->target;

    switch (hilo->state) {
    case HILO_STATE_ENTER:
        if (hilo->frame == 80) {
            start_hilo(hilo);
        }
        break;
    case HILO_STATE_START:
        if (1 <= hilo->frame && hilo->frame < 100) {
            sprintf(hilo->display, "%d       ", get_random_target(hilo));
        } else if (hilo->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO_STATE_PLAY:
        break;
    case HILO_STATE_FLASH:
        if (hilo->frame == 5) {
            go_to_play_state(app);
        }
        break;
    case HILO_STATE_STATS:
        if (hilo->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO_STATE_SHOW_GUESS:
        if (hilo->frame == 25) {
            if (hilo->index == 9) {
                hilo->state = HILO_STATE_LAST_GUESS;
                hilo->frame = 0;
                sprintf(hilo->display, "1 MORE GUESS");
            } else {
                go_to_play_state(app);
            }
        }
        break;
    case HILO_STATE_LAST_GUESS:
        if (hilo->frame == 100) {
            go_to_play_state(app);
        }
        break;
    case HILO_STATE_OVER:
        if (hilo->frame == 1) {
            sprintf(hilo->display, "%03d       ", hilo->guess);
        } else if (hilo->frame == 50 && did_win) {
            char *str = "GUESSES";
            if (hilo->index == 1) str = "GUESS";
            sprintf(hilo->display, "%d %s", hilo->index, str);
        } else if (hilo->frame == 50 && !did_win) {
            sprintf(hilo->display, "%03d YOU LOST", hilo->target);
            hilo->frame = -1;
        } else if (hilo->frame == 200 && did_win) {
            sprintf(hilo->display, "%03d YOU WON", hilo->target);
            hilo->frame = -1;
        }
        break;
    }
}

static bool is_animating(app_t *app) {
    hilo_t *hilo = (hilo_t *)app;
    return hilo->frame >= 0;
}
