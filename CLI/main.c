#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;

static void set_calc_display(int bgcolor) {
    printf("\033[%d;40m", bgcolor);
}

static void reset_display() {
    printf("\033[39;49m");
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

    for (int i = 0; ; i++) {
        // Wait more or less time, depending on the value of 'inc'.
        while (!is_animating(calc)) {
            pthread_cond_wait(&wait_cond, &wait_mutex);
        }
        while (is_animating(calc)) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            long nsec = ts.tv_nsec + 10 * 1000L * 1000L;
            ts.tv_sec += nsec / 1000000000L;
            ts.tv_nsec = nsec % 1000000000L;
            pthread_cond_timedwait(&wait_cond, &wait_mutex, &ts);
            advance(calc);
            print_display(calc);

            if (mess == 'q') {
               // Quit.
               pthread_mutex_unlock(&wait_mutex);
               return 0;
            }
        }
        mess = 0;
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
