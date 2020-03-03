typedef struct game_s {
    int target;
    int index;
    int guess;
    int is_number_editing;
    char number_editing[4];
    int animation_frame;
    long wait_ms;
} game_t;

void reset_game(calc_t *calc);
long press_key_game(calc_t *calc, key_t key);
long advance_game(calc_t *calc);
