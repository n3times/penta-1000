#include "app.h"

typedef enum game_state_e {
    GAME_STATE_APPEAR,
    GAME_STATE_START,
    GAME_STATE_PLAY,
    GAME_STATE_OVER,
} game_state_t;

typedef struct game_s {
    app_t app;
    int target;
    int index;
    int guess;
    int is_number_editing;
    char number_editing[4];
    int frame;
    game_state_t state;
} game_t;

void new_game(calc_t *calc);
