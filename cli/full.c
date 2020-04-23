#include "penta1000.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#define SUPPORT_CUSTOM_COLORS 1

#define ANIMATION_MS 10
#define NANO_IN_ONE_MS 1000000L
#define NANO_IN_ONE_S 1000000000L

#define BGCOLOR_RED 91
#define BGCOLOR_WHITE 97

#define SAVE_FILENAME "penta1000.dat"

static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;
static char pressed_key = 0;
static int fgcolor = BGCOLOR_RED;

/* Display. */

static void set_display(int bgcolor) {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[%d;40m", bgcolor);
#endif
}

static void reset_display() {
#if SUPPORT_CUSTOM_COLORS
    printf("\033[39;49m");
#endif
}

static void print_display(p1_t *p1) {
    char *display = p1_get_display(p1);
    printf("  %16s \r", display);
    fflush(stdout);
}

/* Animation Thread. */

static void *animation_loop(void *args) {
    p1_t *p1 = args;

    while (true) {
        // Wait until there is an animation, which may happen after a key press.
        while (true) {
            pthread_mutex_lock(&wait_mutex);
            if (!p1_is_animating(p1)) {
                pthread_cond_wait(&wait_cond, &wait_mutex);
            }
            if (pressed_key == 'q') {
               // Quit.
               pthread_mutex_unlock(&wait_mutex);
               return NULL;
            }
            if (p1_is_animating(p1)) {
                break;
            }
            pthread_mutex_unlock(&wait_mutex);
        }
        pthread_mutex_unlock(&wait_mutex);

        // Animate while 'p1_is_animating' is true.
        while (true) {
            pthread_mutex_lock(&wait_mutex);
            if (!p1_is_animating(p1)) {
                pthread_mutex_unlock(&wait_mutex);
                break;
            }
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            long nsec = ts.tv_nsec + ANIMATION_MS * NANO_IN_ONE_MS;
            ts.tv_sec += nsec / NANO_IN_ONE_S;
            ts.tv_nsec = nsec % NANO_IN_ONE_S;
            pthread_cond_timedwait(&wait_cond, &wait_mutex, &ts);

            if (pressed_key == 'q') {
               // Quit.
               pthread_mutex_unlock(&wait_mutex);
               return NULL;
            }

            p1_advance_frame(p1);
            print_display(p1);
            pthread_mutex_unlock(&wait_mutex);
        }
    }
}

/* State. */

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

    // Read saved object from file.
    char *state_buffer = malloc(state_buffer_size);
    fread(state_buffer, state_buffer_size, 1, file);
    fclose(file);

    p1_t *p1 = p1_new_from_state_buffer(state_buffer);
    free(state_buffer);
    return p1;
}

/* Logging. */

static bool is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

static void print_operation(char *operation) {
    if (strlen(operation) <= 24) {
        printf(" %s\r\n", operation);
    } else {
        int i;
        for (i = 24; i >= 1; i--) {
            if (is_operator(operation[i])) {
                if (operation[i-1] == 'e' || is_operator(operation[i-1])) {
                    // minus/plus sign.
                    continue;
                }
                char c = operation[i];
                operation[i] = 0;
                printf(" %s\r\n", operation);
                operation[i] = c;
                break;
            }
        }
        print_operation(operation + i);
    }
}

static void print_log(p1_t *p1) {
    long first_available_index = p1_log_get_first_available_index(p1);
    long last_available_index = p1_log_get_last_available_index(p1);
    reset_display();
    printf("\n\n** LOG **\r\n\n");
    bool empty = first_available_index == 0;
    if (empty) {
        printf("[EMPTY]\r\n\n");
    } else {
        printf("-------------------------\r\n");
    }
    for (long i = first_available_index; i <= last_available_index;
         i++) {
        if (!first_available_index) break;
        char entry[2000];
        strcpy(entry, p1_log_get_entry(p1, i));
        char *equal = strchr(entry, '=');
        *equal = 0;
        char *operation = entry;
        char *result = equal + 1;
        if (strlen(operation) + strlen(result) + 2 <= 24) {
            printf(" %s", operation);
            int spaces = 24 - strlen(operation) - strlen(result);
            for (int j = 0; j < spaces; j++) printf(" ");
            printf("%s\r\n", result);
            printf("-------------------------\r\n");
        } else {
            print_operation(operation);
            printf(" %24s\r\n", result);
            printf("-------------------------\r\n");
        }
    }
    printf("\n");
    set_display(fgcolor);
}

/* Main. */

int main(int argc, char *argv[]) {
    printf("\nPentatronics 1000 game/calculator\n\n");
    printf("  'q' quit\n");
    printf("  'g' game\n");
    printf("  'c' clear/calculator\n");
    printf("  '~' change sign\n\n");

    // Restore previous session if possible.
    // Otherwise, get new object.
    p1_t *p1 = load_session(SAVE_FILENAME);
    if (!p1) {
        time_t t;
        srand((unsigned) time(&t));
        p1 = p1_new(t);
    }

    if (argc == 2) fgcolor = atoi(argv[1]);
    set_display(fgcolor);

    print_display(p1);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    // Start animation.
    pthread_t animation_thread;
    pthread_create(&animation_thread, 0, animation_loop, p1);

    // Get user input.
    while (true) {
        pressed_key = tolower(getchar());
        if (pressed_key == '?') pressed_key = 'g';
        if (pressed_key == 127) pressed_key = 'c';
        if (pressed_key == 'x') pressed_key = '*';
        if (pressed_key == '\r') pressed_key = '=';

        if (pressed_key == 'q') {
            reset_display();
            system("/bin/stty cooked");
            pthread_cond_signal(&wait_cond);
            break;
        } else if (pressed_key == 'l') {
            print_log(p1);
        } else if (pressed_key == 'k') {
            p1_log_clear(p1);
        }

        pthread_mutex_lock(&wait_mutex);
        p1_press_key(p1, pressed_key);
        print_display(p1);
        pthread_mutex_unlock(&wait_mutex);
        pthread_cond_signal(&wait_cond);
    }

    // After quitting, wait for the animation thread.
    pthread_join(animation_thread, NULL);

    // Save session.
    save_session(p1, SAVE_FILENAME);

    return 0;
}
