/* USAGE:
 *
 *   // Get a new calc object.
 *   calc_t *calc = new_calc(&calc);
 *
 *   // Repeatedly, report any key pressed and update display.
 *   press_key(calc, <SOME_KEY>);
 *   char display[25];
 *   get_display(calc, display);
 *   // Update display from UI...
 *
 *   // Quit calculator, when done.
 *   release_calc(calc);
 */

typedef struct calc_s calc_t;

calc_t *new_calc();
void release_calc(calc_t *calc);

/* INPUT */

typedef enum {
    KEY_0       = 0,
    KEY_1       = 1,
    KEY_2       = 2,
    KEY_3       = 3,
    KEY_4       = 4,
    KEY_5       = 5,
    KEY_6       = 6,
    KEY_7       = 7,
    KEY_8       = 8,
    KEY_9       = 9,
    KEY_DOT     = 10,
    KEY_CHS     = 11,
    KEY_PLUS    = 12,
    KEY_MINUS   = 13,
    KEY_TIMES   = 14,
    KEY_DIVIDE  = 15,
    KEY_EQUAL   = 16,
    KEY_PERCENT = 17,
    KEY_CLEAR   = 18,
    KEY_GAME    = 19,
} key_t;

int press_key(calc_t *calc, key_t key);

/* OUTPUT */

void get_display(calc_t *calc, char *display);
