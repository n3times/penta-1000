#ifndef CALC_H
#define CALC_H

#include "app.h"

#include <stdbool.h>

typedef enum calc_state_e {
    CALC_STATE_ENTER,
    CALC_STATE_COMPUTE,
} calc_state_t;

typedef enum error_e {
    ERROR_NONE = 0,
    ERROR_ILLEGAL_OP = 1,
    ERROR_OVERFLOW = 2,
    ERROR_OUT_OF_MEM = 3,
} error_t;

typedef struct operand_s {
    double number;
    bool has_percent;
} operand_t;

typedef struct aos_s {
    int stack_depth;
    operand_t operands[100];
    char operators[100];
    error_t error;
} aos_t;

typedef struct log_s {
    long first_index;
    long last_index;
    int entry_offsets[100];
    char mem[2000];
} log_t;

typedef struct calc_s {
    app_t app;
    calc_state_t state;
    int frame;
    bool is_number_editing;
    char number_editing[25];
    aos_t aos;
    log_t log;
} calc_t;

void aos_print(calc_t *calc, char *print);
void get_calc_display(calc_t *calc, char *display);

void init_calc(p1_t *p1);

#endif // CALC_H
