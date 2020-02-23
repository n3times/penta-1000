#include "engine.h"

struct state_s {
    int is_new;
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
