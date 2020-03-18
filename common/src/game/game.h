#include "app.h"

#include <stdbool.h>

typedef enum game_state_e {
    // App has been entered.
    GAME_STATE_ENTER,
    // Start animation sequence of a new game.
    GAME_STATE_START,
    // Game is waiting for user guess.
    GAME_STATE_PLAY,
    // Last guess animation sequence.
    GAME_STATE_LAST_GUESS,
    // Game is over and the display has the end result, possibly animating.
    GAME_STATE_OVER,
} game_state_t;

typedef struct game_s {
    // Pointers to the app methods. Note that they need to be set at creation
    // and deserialization time.
    app_t app;
    // For the random number generator.
    long rng;
    // Mystery number the user has to guess.
    int target;
    // Latest user guess.
    int guess;
    // Number of guesses in the current game.
    int index;
    // The text field where the user inputs their guess. Starts with "---" and
    // ends up with 3 digits.
    char guess_textfield[4];
    // The user is inputing their guess.
    bool is_guess_editing;
    // The current state of the game.
    game_state_t state;
    // The current frame animation within the current state.
    int frame;
} game_t;

void init_game(p1_t *p1, long seed);
