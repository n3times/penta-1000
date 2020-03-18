#include "calc.h"

// Handles a key press while the user is editing a number.
void number_edit_handle_key(calc_t *calc, char key);

// Tells the calculator that a number has been edited and can be pushed into
// the stack.
void number_edit_done_editing(calc_t *calc);
