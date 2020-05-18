#include "app.h"

#include <stdbool.h>

typedef enum hilo2_state_e {
    // App has been entered.
    HILO2_STATE_ENTER,
    // Starts new game.
    HILO2_STATE_START_GAME,
    // Starts new level within game.
    HILO2_STATE_START_LEVEL,
    // User is playing level.
    HILO2_STATE_PLAY_LEVEL,
    // User won level.
    HILO2_STATE_WON_LEVEL,
    // User won game.
    HILO2_STATE_WON_GAME,
    // Game is over.
    HILO2_STATE_GAME_OVER,
    // Flashes display.
    HILO2_STATE_SUBROUTINE_FLASH,
    // Displays stats about game.
    HILO2_STATE_SUBROUTINE_STATS,
} hilo2_state_t;

typedef struct hilo2_s {
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
    // The current state of the game.
    hilo2_state_t state;
    // The current frame animation within the current state.
    int frame;
    // The display.
    char display[25];
    // Current level.
    int level;
    // Score for current game.
    int score;
    // Maximum number of guesses for current level.
    int max_guesses;
    // Maximum time for current level in hundreds of a second.
    int max_time;
    // Time left in hundreds of a second.
    int time;
    // Best score.
    int hiscore;
    //
    bool is_hiscore;
    // State pushed on the stack.
    hilo2_state_t pushed_state;
    // Frame pushed on the stack.
    int pushed_frame;
    // Display pushed on the stack.
    char pushed_display[25];
} hilo2_t;

// Initializes the different fields of a hilo2_t objet, using a given seed for
// its random number generator.
void init_hilo2(hilo2_t *hilo2, long seed);

// Initializes the different fields of a hilo2_t objet from a given state.
void init_hilo2_from_state(char *state);
