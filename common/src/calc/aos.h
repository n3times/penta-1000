#include "calc.h"

// Evaluates the whole arithmetic stack. The result (an operand_t) is placed as
// the single element of the stack.
void aos_eval(aos_t *aos);

// Pushes a number into the stack. The corresponding operand does not have a
// percent sign but the user can add one later.
void aos_push_number(aos_t *aos, double number);

// "Pushes" an arithmetic operator, percent, change of sign or equal into the
// stack.
void aos_push_operator(calc_t *calc, char op);

// Removes the top element of the stack. Called when the user presses the CLEAR
// key.
void aos_pop(aos_t *aos);
