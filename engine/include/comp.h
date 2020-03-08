#include "app.h"

#include <stdbool.h>

typedef enum comp_state_e {
    COMP_STATE_ENTER,
    COMP_STATE_COMPUTE,
} comp_state_t;

typedef enum error_e {
    ERROR_NONE = 0,
    ERROR_ILLEGAL_OP = 1,
    ERROR_OVERFLOW = 2,
} error_t;

typedef struct aos_s {
    int stack_depth;
    double operands[100];
    key_t operators[100];
} aos_t;

typedef struct comp_s {
    app_t app;
    comp_state_t state;
    int frame;
    error_t error;
    bool is_number_editing;
    char number_editing[25];
    aos_t aos;
} comp_t;

void handle_op(calc_t *calc, key_t op);
void edit_number(calc_t *calc, key_t key);
void resolve_edit_number(calc_t *calc);

void new_comp(calc_t *calc);
