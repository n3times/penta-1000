#include "common.h"

#include <stdio.h>
#include <string.h>

static void skip_animations(p1_t *p1) {
    for (int i = 0; i < 500; i++) {
        if (!p1_is_animating(p1)) break;
        p1_advance_frame(p1);
    }
}

static char *get_display_for_key_sequence(p1_t *p1, char *keys) {
    for (int i = 0; i < strlen(keys); i++) {
        p1_press_key(p1, keys[i]);
        skip_animations(p1);
    }
    return p1_get_display(p1);
}

void test(char *keys, char *expected) {
    p1_t *p1 = p1_new(0);
    char *actual = get_display_for_key_sequence(p1, keys);
    printf("\nsequence: %s\n", keys);
    printf("actual display: %s\n", actual);
    if (strcmp(actual, expected)) {
        printf("\nError!!! expected: %s\n\n", expected);
    }
}

test_t *init_test(long seed) {
    printf("\n");
    return p1_new(seed);
}

void press_sequence(test_t *test, char *sequence) {
    printf("sequence: %s\n", sequence);
    for (int i = 0; i < strlen(sequence); i++) {
        p1_press_key(test, sequence[i]);
        skip_animations(test);
    }
}

void press_key(test_t *test, char key) {
    printf("key: %c\n", key);
    p1_press_key(test, key);
}

void advance(test_t *test, long ms) {
    for (long i = 0; i < ms/10; i++) p1_advance_frame(test);
}

void assert_display(test_t *test, char *expected) {
    char *actual = p1_get_display(test);
    printf("actual display: %s\n", actual);
    if (strcmp(actual, expected)) {
        printf("\nError!!! expected display: %s\n\n", expected);
    }
}
