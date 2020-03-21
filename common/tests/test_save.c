#include "penta1000.h"

#include "common.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAVE_FILENAME "test.dat"

/* Saving and loading of state. */

static void save_session(p1_t *p1, char *filename) {
    long size;
    char *state = p1_get_state(p1, &size);

    FILE *file = fopen(filename, "w");
    fwrite(state, size, 1, file);
    fclose(file);
    free(state);
}

static p1_t *load_session(char *filename) {
    // Make sure there is a saved session.
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // Get size of saved object.
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // Read state from file.
    char *state = malloc(size);
    fread(state, size, 1, file);
    fclose(file);

    p1_t *p1 = p1_new_from_state(state);
    free(state);
    return p1;
}

void test_saving() {
    p1_t *p1 = p1_new(0);
    p1_press_key(p1, '7');
    save_session(p1, SAVE_FILENAME);
    p1_t *new_p1 = load_session(SAVE_FILENAME);
    assert_display(new_p1, "7");
}
