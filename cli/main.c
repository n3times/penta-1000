#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void set_calc_display() {
    printf("\033[91;40m               \r");
}

static void reset_display() {
    printf("\033[39;49m               \r");
}

int main() {
    calc_t *calc = 0;
    char display[25];

    new_calc(&calc);

    set_calc_display();

    get_display(calc, display);
    printf("  %12s\r", display);

    while (1) {
        // The system calls make sure the pressed characters are not buffered
        // and therefore are immediately available (without the user pressing
        // the return key).
        system("/bin/stty raw");
        char c = tolower(getchar());
        system("/bin/stty cooked");

        if (c == 'q') {
            reset_display();
            exit(0);
        }

	char *char_to_key_map = "0123456789.~+-*/=%cg";
        for (int i = 0; i < strlen(char_to_key_map); i++) {
            if (char_to_key_map[i] == c) {
                if (press_key(calc, i)) {
                    get_display(calc, display);
                    printf("  %12s\r", display);
                }
                break;
            }
        }
    }
}
