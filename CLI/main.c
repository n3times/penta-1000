#include "penta1000.h"

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

static void set_display(int bgcolor) {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[%d;40m", bgcolor);
#endif
}

static void reset_display() {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[39;49m");
#endif
}

static void print_display(p1000_t *p1000) {
    char *display = p1000_get_display(p1000);
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
    p1000_t *p1000 = args;

    while (true) {
        while (!p1000_is_animating(p1000)) {
            pthread_cond_wait(&wait_cond, &wait_mutex);
        }
        while (p1000_is_animating(p1000)) {
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

            p1000_advance_frame(p1000);
            print_display(p1000);
        }
        pthread_mutex_unlock(&wait_mutex);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    p1000_t *p1000 = NULL;
    pthread_t animation_thread;

    mess = 0;

    p1000 = p1000_new();

    pthread_create(&animation_thread, 0, animation_loop, p1000);

    int fgcolor = 97;
    if (argc == 2) fgcolor = atoi(argv[1]);
    set_display(fgcolor);

    print_display(p1000);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    while (1) {
        char c = tolower(getchar());
        mess = c;

        if (c == 'q') {
            quit();
        }

        p1000_press_key(p1000, c);
        print_display(p1000);
        pthread_cond_signal(&wait_cond);
    }
}