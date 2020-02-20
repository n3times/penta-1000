#include "engine.h"

struct state_s {
    int is_error;
    int stack_depth;
    double current_number;
    key_t current_op;
    double next_number;
    int is_number_editing;
    char number_editing[24];
};

void compute(state_t *state, key_t op);
void edit_number(state_t *state, key_t key);
void resolve_edit_number(state_t *state);
