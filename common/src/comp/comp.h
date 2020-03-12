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

typedef struct operand_s {
    double number;
    bool has_percent;
} operand_t;

typedef struct aos_s {
    int stack_depth;
    operand_t operands[100];
    char operators[100];
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

void edit_number(p1000_t *p1000, char key);
void resolve_edit_number(p1000_t *p1000);
void get_p1000_display(p1000_t *p1000, char *display);

void aos_push_number(p1000_t *p1000, double number);
void aos_push_operator(p1000_t *p1000, char op);
void aos_pop(p1000_t *p1000);
void aos_eval(p1000_t *p1000);

void new_comp(p1000_t *p1000);
