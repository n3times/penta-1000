#include "engine.h"

#include <stdio.h>
#include <string.h>

static void print_display(state_t *state) {
    char display[100];    
    get_display(state, display);
    printf("%s\n", display);
}

int main() {
    state_t *state;
    new_state(&state);

    key_t keys[] = {
        KEY_0,
        KEY_DIVIDE,
        KEY_0,
        KEY_EQUAL,
        KEY_CLEAR,
    };

    print_display(state);
    for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) { 
       press_key(state, keys[i]);
       print_display(state);
    }
}
