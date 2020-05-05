#include "app.h"

#include <stdbool.h>

typedef enum hilo_state_e {
    // App has been entered.
    HILO_STATE_ENTER,
    // Start animation sequence of a new game.
    HILO_STATE_START,
    // Game is waiting for user guess.
    HILO_STATE_PLAY,
    // Brief flash indicating a key press has been ignored.
    HILO_STATE_FLASH,
    // Game is showing statistics about the game.
    HILO_STATE_STATS,
    // Guess has been entered and is being shown.
    HILO_STATE_SHOW_GUESS,
    // Last guess animation sequence.
    HILO_STATE_LAST_GUESS,
    // Game is over and the display has the end result, possibly animating.
    HILO_STATE_OVER,
} hilo_state_t;

typedef struct hilo_s {
    // Pointers to the app methods. Note that they need to be set at creation
    // time and when the game is recreated from a state.
    app_t app;
    // Used by the random number generator.
    long rng;
    // Mystery number the user has to guess.
    int target;
    // Latest user guess.
    int guess;
    // Number of guesses in the current game.
    int index;
    // The text field where the user inputs the guess. Starts with "---" and
    // ends up with 3 digits.
    char guess_textfield[4];
    // The user is inputing the guess.
    bool is_guess_editing;
    // The current state of the game.
    hilo_state_t state;
    // The current frame animation within the current state.
    int frame;
    // The display.
    char display[25];
} hilo_t;

// Initializes the different fields of a hilo_t objet, using a given seed for
// its random number generator.
void init_hilo(hilo_t *hilo, long seed);

// Initializes the different fields of a hilo_t objet from a given state.
void init_hilo_from_state(char *state);
