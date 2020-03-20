#include "common.h"

#include <stdio.h>
#include <string.h>

static void ignore_animations(p1_t *p1) {
    for (int i = 0; i < 500; i++) {
        if (!p1_is_animating(p1)) break;
        p1_advance_frame(p1);
    }
}

static char *get_display_for_key_sequence(p1_t *p1, char *keys) {
    for (int i = 0; i < strlen(keys); i++) {
        p1_press_key(p1, keys[i]);
        ignore_animations(p1);
    }
    return p1_get_display(p1);
}

void test(char *keys, char *expected) {
    p1_t *p1 = p1_new(0);
    char * result = get_display_for_key_sequence(p1, keys);
    printf("%s %s\n", keys, result);
    if (strcmp(result, expected)) {
        printf("\nError!!! result: %s expected: %s\n\n", result, expected);
    }
}

