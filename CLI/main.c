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

static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;
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
    printf("  %12s \r", display);
    fflush(stdout);
}

static char pressed_key = 0;

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

// Stores Pentatronics 1000 object in file.
static void serialize(p1_t *p1, char *filename) {
    // Get p1 as raw data.
    long raw_data_size;
    void *raw_data = p1_get_raw_data(p1, &raw_data_size);

    // Store raw data into file.
    FILE *file = fopen(filename, "w");
    fwrite(raw_data, raw_data_size, 1, file);
    fclose(file);
}

// Retrieves Pentatronics 1000 object from file.
static p1_t *deserialize(char *filename) {
    // Get size of raw data.
    long raw_data_size;
    (void)p1_get_raw_data(NULL, &raw_data_size);

    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // Read raw data from file.
    void *raw_data = malloc(raw_data_size);
    fread(raw_data, raw_data_size, 1, file);
    fclose(file);

    p1_t *p1 = p1_restore_from_raw_data(raw_data);
    return p1;
}

int main(int argc, char *argv[]) {
    p1_t *p1 = deserialize("penta1000.dat");
    if (!p1) {
        time_t t;
        srand((unsigned) time(&t));
        p1 = p1_new(t);
    }

    int fgcolor = BGCOLOR_RED;
    if (argc == 2) fgcolor = atoi(argv[1]);
    set_display(fgcolor);

    print_display(p1);

    // Make sure the pressed keys are immediately available, without the user
    // having to press the return key.
    system("/bin/stty raw");

    pthread_t animation_thread;
    pthread_create(&animation_thread, 0, animation_loop, p1);

    while (true) {
        pressed_key = tolower(getchar());

        if (pressed_key == 'q') {
            reset_display();
            system("/bin/stty cooked");
            pthread_cond_signal(&wait_cond);
            break;
        }

        pthread_mutex_lock(&wait_mutex);
        p1_press_key(p1, pressed_key);
        print_display(p1);
        pthread_mutex_unlock(&wait_mutex);
        pthread_cond_signal(&wait_cond);
    }
    pthread_join(animation_thread, NULL);
    serialize(p1, "penta1000.dat");
    return 0;
}
