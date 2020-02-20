typedef struct state_s state_t;

void new_state(state_t **state);

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
} key_t;

void press_key(state_t *state, key_t key); 

/* OUTPUT */

void get_display(state_t *state, char *display);
