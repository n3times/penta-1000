#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>

static void set_calc_display() {
    printf("\033[91;40m");
}

static void reset_display() {
    printf("\033[39;49m");
}

static void quit() {
    reset_display();
    system("/bin/stty cooked");
    exit(0);
}

int main() {
    calc_t *calc = 0;
    char *display = 0;

    calc = new_calc();

    set_calc_display();

    display = get_display(calc);
    printf("  %12s \r", display);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    while (1) {
        char c = tolower(getchar());

        if (c == 'q') {
            quit();
        }

        char *char_to_key_map = "0123456789.~+-*/=%cg";
        for (int i = 0; i < strlen(char_to_key_map); i++) {
            if (char_to_key_map[i] == c) {
                if (press_key(calc, i)) {
                    display = get_display(calc);
                    printf("  %12s\r", display);
                    int ms;
                    while ((ms = get_ms_wait_to_advance(calc))) {
                        usleep(ms * 1000);
                        advance(calc);
                        display = get_display(calc);
                        printf("  %12s\r", display);
                        fflush(stdout);
                    }
                }
                break;
            }
        }
    }
}
