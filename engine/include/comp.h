typedef enum error_e {
    ERROR_NONE = 0,
    ERROR_ILLEGAL_OP = 1,
    ERROR_OVERFLOW = 2,
} error_t;

typedef struct aos_s {
    int stack_depth;
    double number_1;
    key_t op_1;
    double number_2;
    key_t op_2;
    double number_3;
} aos_t;

typedef struct comp_s {
    error_t error;
    int is_number_editing;
    char number_editing[25];
    aos_t aos;
} comp_t;

void handle_op(calc_t *calc, key_t op);
void edit_number(calc_t *calc, key_t key);
void resolve_edit_number(calc_t *calc);

long press_key_comp(calc_t *calc, key_t key);
long advance_comp(calc_t *calc);
