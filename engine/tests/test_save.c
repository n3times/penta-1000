#include "penta1000.h"

#include "common.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAVE_FILENAME "test.dat"

/* Saving and loading of state. */

static void save_session(p1_t *p1, char *filename) {
    long state_buffer_size = p1_get_state_buffer_size(p1);
    char *state_buffer = p1_get_state_buffer(p1);

    FILE *file = fopen(filename, "w");
    fwrite(state_buffer, state_buffer_size, 1, file);
    fclose(file);
    p1_release_state_buffer(state_buffer);
}

static p1_t *load_session(char *filename) {
    // Make sure there is a saved session.
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // Get size of saved object.
    fseek(file, 0L, SEEK_END);
    long state_buffer_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // Read state from file.
    char *state_buffer = malloc(state_buffer_size);
    fread(state_buffer, state_buffer_size, 1, file);
    fclose(file);

    p1_t *p1 = p1_new_from_state_buffer(state_buffer);
    free(state_buffer);
    return p1;
}

void test_saving() {
    p1_t *p1 = p1_new(0);
    p1_press_key(p1, '7');
    save_session(p1, SAVE_FILENAME);
    p1_t *new_p1 = load_session(SAVE_FILENAME);
    assert_display(new_p1, "7");
}
