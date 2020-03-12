/**
 * API that clients can use as a black box to implement a Pentatronics 1000
 * emulator.
 *
 * The client should use 2 threads, one (possibly the main thread) to handle
 * user input (key presses) and another one to run the display animations.
 *
 * The animation thread should call "p1000_advance_frame" every 10 ms as long as
 * "p1000_is_animating" is true.
 */

#include <stdbool.h>

typedef struct p1000_s p1000_t;

/* SETUP */

// Allocates a new Pentatronics 1000 object and inits it.
// After call, update display and start animating if necessary.
p1000_t *p1000_new();

// Deallocates p1000.
void p1000_release(p1000_t *p1000);

/* INPUT */

typedef enum {
    KEY_0       = 0,
    KEY_1       = 1,
    KEY_2       = 2,
    KEY_3       = 3,
    KEY_4       = 4,
    KEY_5       = 5,
    KEY_6       = 6,
    KEY_7       = 7,
    KEY_8       = 8,
    KEY_9       = 9,
    KEY_DOT     = 10,
    KEY_CHS     = 11,
    KEY_PLUS    = 12,
    KEY_MINUS   = 13,
    KEY_TIMES   = 14,
    KEY_DIVIDE  = 15,
    KEY_EQUAL   = 16,
    KEY_PERCENT = 17,
    KEY_CLEAR   = 18,
    KEY_GAME    = 19,
} key_t;

// Should be called as soon as a key is pressed.
// After call, update display and check animation.
void p1000_press_key(p1000_t *p1000, key_t key);

/* OUTPUT */

// Gets the display as a null terminated string with at most 24 effective
// characters.
//
// The text should be right justified on the display. A dot ('.') modifies the
// character just before it.
char *p1000_get_display(p1000_t *p1000);

/* ANIMATION */

// Should be called every 10 ms.
// After call, update display and check animation.
void p1000_advance_frame(p1000_t *p1000);

// True if animate() should be called in the animation thread. 
bool p1000_is_animating(p1000_t *p1000);
