typedef enum game_state_e {
    GAME_STATE_APPEAR,
    GAME_STATE_START,
    GAME_STATE_PLAY,
    GAME_STATE_OVER,
} game_state_t;

typedef struct game_s {
    int target;
    int index;
    int guess;
    int is_number_editing;
    char number_editing[4];
    int frame;
    game_state_t state;
} game_t;

void appear_game(calc_t *calc);

void press_key_game(calc_t *calc, key_t key);
void advance_game(calc_t *calc);
bool is_animating_game(calc_t *calc);
