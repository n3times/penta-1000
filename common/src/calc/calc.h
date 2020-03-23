#ifndef CALC_H
#define CALC_H

#include "app.h"

#include <stdbool.h>

typedef enum calc_state_e {
    // Enter animation sequence.
    CALC_STATE_ENTER,
    // Standard state either accepting user input or evaluating operation.
    CALC_STATE_COMPUTE,
    CALC_STATE_ERROR,
} calc_state_t;

typedef enum error_e {
    ERROR_NONE = 0,
    // In practice, division by zero.
    ERROR_ILLEGAL_OP = 1,
    // Final or intermediary result is too large.
    ERROR_OVERFLOW = 2,
    // Not enough memory to hold all the operands and operators of the current
    // operation.
    ERROR_OUT_OF_MEM = 3,
} error_t;

typedef struct operand_s {
    double number;
    bool has_percent;
} operand_t;

typedef struct aos_s {
    // The depth of the stack (between 0 and 200). Stack alternates between
    // operand and operator, starting with an operand.
    int stack_depth;
    // The operands in the stack.
    operand_t operands[100];
    // The arithmetic operators in the stack.
    char operators[100];
    // Error, if any, while trying to evaluate the last operation.
    error_t error;
} aos_t;

typedef struct log_s {
    // Index of the first available log entry.
    long first_index;
    // Index of the last available log entry.
    long last_index;
    // The list of offsets within 'mem' of the available log entries.
    int entry_offsets[100];
    // Contains the available log entries, each one being null-terminated.
    char mem[2000];
} log_t;

typedef struct calc_s {
    // Pointers to the app methods. Note that they need to be set at creation
    // time and when the calc is recreates from a state.
    app_t app;
    // The current state of the calculator.
    calc_state_t state;
    // The current frame animation within the current state.
    int frame;
    // The user is editing a number within an operation.
    bool is_number_editing;
    // The number being edited.
    char number_editing[25];
    // The operation stack (AOS = Algebraic Operating System).
    aos_t aos;
    // Log entries, each one of the form operation=result. For example "1+1=2"
    // or "10+10%+10%=12.1".
    log_t log;
    // The display.
    char display[25];
} calc_t;

void init_calc(calc_t *calc);
void init_calc_from_state(char *state);

#endif // CALC_H
