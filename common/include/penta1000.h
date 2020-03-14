/**
 * API for clients that want to implement a Pentatronics 1000 emulator.
 *
 * Clients should use 2 threads: one, possibly the main thread, to handle
 * user input (key presses) and another one to run the display animations.
 *
 * The animation thread should call "p1_advance_frame" every 10 ms as long as
 * "p1_is_animating" is true.
 */

#include <stdbool.h>

typedef struct p1_s p1_t;

// Allocates a new Pentatronics 1000 object and inits it.
//
// After the allocation, the client should update the display and start
// animations if necessary.
p1_t *p1_new(long seed);

// Deallocates the Pentatronics 1000 object.
void p1_release(p1_t *p1);

// Reports a key press by the user.
//
// After calling this method, clients should update the display and perform
// animations as necessary.
//
// "key" is in "0123456789.~+-*/=%cg" with the 10 digits, the 4 arithmetic
// operations, percent, equal, dot and:
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

// Advances the animation by 1 frame.
//
// The client should call this method  every 10 ms if "p1_is_animating" is true.
// Afterward, the client should update the display and perform animations as
// necessary.
void p1_advance_frame(p1_t *p1);

// Returns true if "p1_advance_frame" should be called.
bool p1_is_animating(p1_t *p1);

// Gets the raw data of a p1_t object as an array of bytes.
//
// This data can be used for serialization.
void *p1_get_raw_data(p1_t *p1, long *byte_count_out);

// Restores a p1_t object from raw data.
//
// This can be used by the client for deserializion.
p1_t *p1_restore_from_raw_data(void *raw);
