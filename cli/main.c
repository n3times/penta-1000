#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>

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

static int is_animating;

void *animation_loop(void *args) {
    calc_t *calc = args;
    long ms = 10;
    while (ms) {
        usleep(ms * 1000);
        ms = advance(calc);
        print_display(calc);
    }
    is_animating = 0;
    return 0;
}

int main(int argc, char *argv[]) {
    calc_t *calc = 0;
    pthread_t animation_thread;

    calc = new_calc();

    int fgcolor = 91;
    if (argc == 2) fgcolor = atoi(argv[1]);
    set_calc_display(fgcolor);

    print_display(calc);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    while (1) {
        char c = tolower(getchar());

        if (is_animating) continue;

        if (c == 'q') {
            quit();
        }

        char *char_to_key_map = "0123456789.~+-*/=%cg";
        for (int i = 0; i < strlen(char_to_key_map); i++) {
            if (char_to_key_map[i] == c) {
                long ms = press_key(calc, i);
                print_display(calc);
                if (ms) {
                    is_animating = 1;
                    pthread_create(&animation_thread, 0, animation_loop, calc);
                }
                break;
            }
        }
    }
}
