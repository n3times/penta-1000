#include "calc.h"

// Evaluates the whole arithmetic stack.
// The result (an operand_t) is placed as the single element of the stack.
void aos_eval(aos_t *aos);

void aos_push_number(aos_t *aos, double number);
void aos_push_operator(calc_t *calc, char op);
void aos_pop(aos_t *aos);
