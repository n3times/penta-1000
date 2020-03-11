#include "app.h"

#include <stdbool.h>

typedef enum game_state_e {
    GAME_STATE_ENTER,
    GAME_STATE_INIT,
    GAME_STATE_PLAY,
    GAME_STATE_LAST_GUESS,
    GAME_STATE_OVER,
} game_state_t;

typedef struct game_s {
    app_t app;
    int target;
    int guess;
    int index;
    char guess_textfield[4];
    bool is_guess_editing;
    game_state_t state;
    int frame;
} game_t;

void init_game(calc_t *calc);
