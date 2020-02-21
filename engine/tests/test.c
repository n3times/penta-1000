#include "engine.h"

#include <stdio.h>
#include <string.h>

static char *key_strings = "0123456789.~+-*/=%C";

static void print_display(state_t *state) {
    char display[100];    
    get_display(state, display);
    printf("%s\n", display);
}

static void print_key(key_t key) {
    printf("[%c]   ", key_strings[key]);
}

static void test_sequence(key_t *sequence, int count, char *title) {
    state_t *state;
    new_state(&state);

    printf("=== %s ===\n\n", title);
    print_display(state);
    for (int i = 0; i < count; i++) { 
       press_key(state, sequence[i]);
       print_key(sequence[i]);
       print_display(state);
    }
    printf("\n");
}

static void test_division_by_zero() {
    key_t sequence[] = {
        KEY_0,
        KEY_DIVIDE,
        KEY_0,
        KEY_EQUAL,
    };
    int count = sizeof(sequence)/sizeof(sequence[0]);
    char *title = "Division by zero";

    test_sequence(sequence, count, title);
}

static void test_arithmetic() {
    key_t sequence[] = {
        KEY_2,
        KEY_PLUS,
        KEY_3,
        KEY_PLUS,
        KEY_4,
        KEY_EQUAL,
        KEY_TIMES,
        KEY_4,
        KEY_MINUS,
        KEY_3,
        KEY_DIVIDE,
        KEY_3,
        KEY_EQUAL,
    };

    int count = sizeof(sequence)/sizeof(sequence[0]);
    char *title = "Arithmetic";

    test_sequence(sequence, count, title);
}

static void test_clear() {
    key_t sequence[] = {
        KEY_2,
        KEY_CLEAR,
        KEY_2,
        KEY_PLUS,
        KEY_CLEAR,
        KEY_2,
        KEY_PLUS,
        KEY_2,
        KEY_2,
        KEY_CLEAR,
    };
    int count = sizeof(sequence)/sizeof(sequence[0]);
    char *title = "Clear";

    test_sequence(sequence, count, title);
}

static void test_percent() {
    key_t sequence[] = {
        KEY_1,
        KEY_1,
        KEY_TIMES,
        KEY_2,
        KEY_0,
        KEY_0,
        KEY_PERCENT,
        KEY_EQUAL,
        KEY_CLEAR,
        KEY_2,
        KEY_0,
        KEY_0,
        KEY_PLUS,
        KEY_5,
        KEY_PERCENT,
        KEY_EQUAL,
    };
    int count = sizeof(sequence)/sizeof(sequence[0]);
    char *title = "Percent";

    test_sequence(sequence, count, title);
}

int main() {
    test_arithmetic();
    test_clear();
    test_division_by_zero();
    test_percent();
}
