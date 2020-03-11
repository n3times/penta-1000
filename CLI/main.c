#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#define SUPPORT_CUSTOM_COLORS 1

#define ANIMATION_MS 10
#define NANO_IN_ONE_MS 1000000L
#define NANO_IN_ONE_S 1000000000L

static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;

static void set_calc_display(int bgcolor) {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[%d;40m", bgcolor);
#endif
}

static void reset_display() {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[39;49m");
#endif
}

static void print_display(calc_t *calc) {
    char *display = get_display(calc);
    printf("  %12s \r", display);
    fflush(stdout);
}

static void quit() {
    reset_display();
    system("/bin/stty cooked");
    exit(0);
}

static char mess = 0;

static void *animation_loop(void *args) {
    calc_t *calc = args;

    while (true) {
        while (!is_animating(calc)) {
            pthread_cond_wait(&wait_cond, &wait_mutex);
        }
        while (is_animating(calc)) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            long nsec = ts.tv_nsec + ANIMATION_MS * NANO_IN_ONE_MS;
            ts.tv_sec += nsec / NANO_IN_ONE_S;
            ts.tv_nsec = nsec % NANO_IN_ONE_S;
            mess = 0;
            pthread_cond_timedwait(&wait_cond, &wait_mutex, &ts);

            if (mess == 'q') {
               // Quit.
               pthread_mutex_unlock(&wait_mutex);
               return 0;
            }

            advance_frame(calc);
            print_display(calc);
        }
        pthread_mutex_unlock(&wait_mutex);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    calc_t *calc = NULL;
    pthread_t animation_thread;

    mess = 0;

    calc = new_calc();

    pthread_create(&animation_thread, 0, animation_loop, calc);

    int fgcolor = 97;
    if (argc == 2) fgcolor = atoi(argv[1]);
    set_calc_display(fgcolor);

    print_display(calc);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    while (1) {
        char c = tolower(getchar());
        mess = c;

        if (c == 'q') {
            quit();
        }

        char *char_to_key_map = "0123456789.~+-*/=%cg";
        for (int i = 0; i < strlen(char_to_key_map); i++) {
            if (char_to_key_map[i] == c) {
                press_key(calc, i);
                print_display(calc);
                pthread_cond_signal(&wait_cond);
                break;
            }
        }
    }
}
