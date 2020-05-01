#include "app.h"

#include <stdbool.h>

typedef enum game_state_e {
    // App has been entered.
    GAME_STATE_ENTER,
    // Start animation sequence of a new game.
    GAME_STATE_START,
    // Game is waiting for user guess.
    GAME_STATE_PLAY,
    // Brief flash indicating a key press has been ignored.
    GAME_STATE_FLASH,
    // Game is showing statistics about the game.
    GAME_STATE_STATS,
    // Guess has been entered and is being shown.
    GAME_STATE_SHOW_GUESS,
    // Last guess animation sequence.
    GAME_STATE_LAST_GUESS,
    // Game is over and the display has the end result, possibly animating.
    GAME_STATE_OVER,
} game_state_t;

typedef struct game_s {
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
    game_state_t state;
    // The current frame animation within the current state.
    int frame;
    // The display.
    char display[25];
} game_t;

// Initializes the different fields of a game_t objet, using a given seed for
// its random number generator.
void init_game(game_t *game, long seed);

// Initializes the different fields of a game_t objet from a given state.
void init_game_from_state(char *state);
