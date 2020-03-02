/**
 * Stopwatch to demonstrate basic usage of POSIX threads.
 *
 * Keys:
 * - 'q' to quit
 * - 'p' to pause/unpause
 * - 'r' to reset
 * - '+' to go faster
 * - '-' to go slower
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct data_s {
    char mess;
} data_t;

static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;

static void *input_loop(void *args) {
    data_t *data = args;

    // Do not buffer key presses.
    system("/bin/stty raw");

    while (1) {
        char c = getchar();
        pthread_mutex_lock(&wait_mutex);
        data->mess = c;
        pthread_mutex_unlock(&wait_mutex);
       
        // Tell animation thread that a key has been pressed. 
        pthread_cond_signal(&wait_cond);
        // Wait until animation thread has handled the key pressed.
        pthread_mutex_lock(&wait_mutex);
        pthread_mutex_unlock(&wait_mutex);

        if (c == 'q') {
            break;
        }
    }
    system("/bin/stty cooked");
    return 0;
}

static void *animation_loop(void *args) {
    data_t *data = args;
    long inc = 50L;

    for (int i = 0; ; i++) {
        printf("%6d :%5ld ms\r", i, inc);
        fflush(stdout);

        // Wait more or less time, depending on the value of 'inc'.
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        long nsec = ts.tv_nsec + inc * 1000L * 1000L;
        ts.tv_sec += nsec / 1000000000L;
        ts.tv_nsec = nsec % 1000000000L;
        pthread_cond_timedwait(&wait_cond, &wait_mutex, &ts);

        if (data->mess == 'q') {
            // Quit.
            pthread_mutex_unlock(&wait_mutex);
            break;
        } else if (data->mess == 'p') {
            // Pause and wait to be unpaused.
            pthread_cond_wait(&wait_cond, &wait_mutex);
            if (data->mess == 'q') {
                pthread_mutex_unlock(&wait_mutex);
                break;
            }
        } else if (data->mess == 'r') {
            // Reset.
            i = 0;
        } else if (data->mess == '-') {
            // Slow down.
            if (inc < 200) inc += 10;
        } else if (data->mess == '+') {
            // Speed up.
            if (inc > 10) inc -= 10;
        }
        data->mess = 0;
        pthread_mutex_unlock(&wait_mutex);
    }
    return 0;
}

int main() {
    pthread_t loop_thread;
    pthread_t animation_thread;

    data_t data;
    data.mess = 0;

    pthread_create(&loop_thread, 0, input_loop, &data);
    pthread_create(&animation_thread, 0, animation_loop, &data);

    pthread_join(animation_thread, 0);
    pthread_join(loop_thread, 0);
}
