/**
 * API that clients can use to implement a Pentatronics 1000 emulator.
 *
 * The client should use 2 threads: one, possibly the main thread, to handle
 * user input (key presses) and another one to run the display animations.
 *
 * The animation thread should call "p1_advance_frame" every 10 ms as long as
 * "p1_is_animating" is true.
 */

#include <stdbool.h>

typedef struct p1_s p1_t;

// Allocates a new Pentatronics 1000 object and inits it.
// After call, update display and start animating if necessary.
p1_t *p1_new();

// Deallocates the Pentatronics 1000 object.
void p1_release(p1_t *p1);

// Should be called as soon as a key is pressed.
// After call, update display and check animation.
//
// key is in '0123456789.~+-*/=%cg' with the 10 digits, the 4 arithmetics
// operations, percentage, equal, dot and:
// ~ for +/-
// c for clear
// g for game (switches between game and calc mode)
void p1_press_key(p1_t *p1, char key);

// Gets the display as a null terminated string with at most 24 effective
// characters.
//
// The text should be right justified on the display. A dot ('.') modifies the
// character just before it.
char *p1_get_display(p1_t *p1);

// Should be called every 10 ms if "p1_is_animating" is true.
// After call, update display and check animation.
void p1_advance_frame(p1_t *p1);

// True if "p1_advance_frame" should be called. 
bool p1_is_animating(p1_t *p1);
