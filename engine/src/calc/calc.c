#include "penta1000_internal.h"

#include "aos.h"
#include "number_edit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void enter_calc(app_t *app);
static void press_key_calc(app_t *app, char key);
static char *get_display_calc(app_t *app);
static void advance_frame_calc(app_t *app);
static bool is_animating_calc(app_t *app);

static bool is_number_edit_key(calc_t *calc, char key) {
    if ('0' <= key && key <= '9') return true;
    if (key == '.') return true;
    if (key == '~') {
        if (calc->aos.stack_depth % 2 == 0) {
            return true;
        }
    }
    return false;
}

static void update_display(calc_t *calc) {
    char extended_display[40];
    memset(extended_display, 0, 40);
    aos_t *aos = &calc->aos;

    if (aos->error == ERROR_DIV_BY_ZERO) {
        strcpy(extended_display, "DIV BY ZERO");
    } else if (aos->error == ERROR_OVERFLOW) {
        strcpy(extended_display, "OVERFLOW");
    } else if (aos->error == ERROR_OUT_OF_MEM) {
        strcpy(extended_display, "OUT OF MEM");
    } else {
        aos_print(calc, extended_display, 20);
        if (calc->is_number_editing) {
            strcat(extended_display, calc->number_editing);
        }
    }
    if (extended_display[0] == '\0') {
        strcpy(extended_display, "READY");
    }
    int offset = strlen(extended_display) - 12;
    if (strchr(extended_display, '.')) offset -= 1;
    if (offset < 0) {
        offset = 0;
    }
    strcpy(calc->display, extended_display + offset);
}

static void setup_app_methods(calc_t *calc) {
    calc->app.enter = enter_calc;
    calc->app.press_key = press_key_calc;
    calc->app.get_display = get_display_calc;
    calc->app.advance_frame = advance_frame_calc;
    calc->app.is_animating = is_animating_calc;
}

void init_calc(calc_t *calc) {
    memset(calc, 0, sizeof(calc_t));
    setup_app_methods(calc);
}

void init_calc_from_state(char *state) {
    setup_app_methods((calc_t *)state);
}

/* App interface. */

static void enter_calc(app_t *app) {
    calc_t *calc = (calc_t *)app;
    sprintf(calc->display, ">  CALC MODE");
    calc->state = CALC_STATE_ENTER;
    calc->frame = 0;
}

static void press_key_calc(app_t *app, char key) {
    if (!strchr("01234567890.~%+-*/=c", key)) return;

    calc_t *calc = (calc_t *)app;
    aos_t *aos = &calc->aos;
    bool is_error = aos->error != ERROR_NONE;

    if (calc->state == CALC_STATE_ENTER) return;
    if (is_error && key != 'c') return;

    if (key == 'c') {
        if (is_error) {
            aos->stack_depth = 0;
            calc->is_number_editing = false;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
            aos->error = ERROR_NONE;
            calc->state = CALC_STATE_COMPUTE;
        } else if (calc->is_number_editing) {
            calc->is_number_editing = false;
            memset(calc->number_editing, 0, sizeof(calc->number_editing));
        } else {
            aos_pop(aos);
        }
    } else if (is_number_edit_key(calc, key)) {
        bool is_error = aos->error != ERROR_NONE;
        if (!is_error) number_edit_handle_key(calc, key);
    } else {
        if (calc->is_number_editing) {
           number_edit_done_editing(calc);
        }
        aos_push_operator(calc, key);
        is_error = aos->error != ERROR_NONE;
        if (is_error) {
            calc->state = CALC_STATE_ERROR;
            calc->frame = 0;
        }
    }

    char old_display[25];
    strcpy(old_display, calc->display);
    update_display(calc);
    char *new_display = calc->display;
    if (!strcmp(new_display, old_display)) {
        strcpy(calc->display, "");
        calc->state = CALC_STATE_FLASH;
        calc->frame = 0;
    }
}

static char *get_display_calc(app_t *app) {
    calc_t *calc = (calc_t *)app;
    return calc->display;
}

static void advance_frame_calc(app_t *app) {
    calc_t *calc = (calc_t *)app;

    calc->frame++;

    switch (calc->state) {
    case CALC_STATE_ENTER:
        if (calc->frame == 80) {
            bool is_error = calc->aos.error != ERROR_NONE;
            calc->state = is_error ? CALC_STATE_ERROR : CALC_STATE_COMPUTE;
            calc->frame = 0;
            update_display(calc);
        }
        break;
    case CALC_STATE_COMPUTE:
        break;
    case CALC_STATE_FLASH:
        if (calc->frame == 1) {
            update_display(calc);
            calc->state = CALC_STATE_COMPUTE;
            calc->frame = 0;
        }
        break;
    case CALC_STATE_ERROR:
        // Animate indefinitely.
        if (calc->frame % 100 == 0) {
            update_display(calc);
        } else if (calc->frame % 100 == 50) {
            sprintf(calc->display, "");
        }
        break;
    }
}

static bool is_animating_calc(app_t *app) {
    calc_t *calc = (calc_t *)app;
    return calc->state != CALC_STATE_COMPUTE;
}
