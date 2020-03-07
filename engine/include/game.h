typedef enum game_state_e {
    GAME_STATE_STARTING,
    GAME_STATE_PLAYING,
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
    bool is_animating;
} game_t;

void reset_game(calc_t *calc);

void press_key_game(calc_t *calc, key_t key);
void advance_game(calc_t *calc);
