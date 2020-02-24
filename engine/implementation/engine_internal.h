#include "engine.h"

typedef struct game_s {
    int target;
    int is_number_editing;
    char number_editing[4];
    int guess;
    int previous_guess;
    int index;
} game_t;

struct state_s {
    int is_new;
    int is_game;
    game_t game;
    int is_data_error;
    int is_overflow;
    int stack_depth;
    double number_1;
    key_t op_1;
    double number_2;
    key_t op_2;
    double number_3;
    int is_number_editing;
    char number_editing[25];
};

void handle_op(state_t *state, key_t op);
void edit_number(state_t *state, key_t key);
void resolve_edit_number(state_t *state);
