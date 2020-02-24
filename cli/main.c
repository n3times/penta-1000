#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *char_to_key_map = "0123456789.~+-*/=%cg";

int main() {
    state_t *state = 0;
    char display[25];

    new_state(&state);

    get_display(state, display);
    printf("[ ]  %12s\n", display);

    while (1) {
        system("/bin/stty raw");
        char c = tolower(getchar());
        system("/bin/stty cooked");

        if (c == 'q') exit(0);

        for (int i = 0; i < strlen(char_to_key_map); i++) {
            if (char_to_key_map[i] == c) {
                if (press_key(state, i)) {
                    get_display(state, display);
                    printf("[%c]  %12s\n", c, display);
                }
                break;
            }
        }
    }
}
