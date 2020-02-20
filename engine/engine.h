typedef struct state_s state_t;

/* INPUT */

typedef enum(
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
    KEY_CHS     = 11
    KEY_PLUS    = 12,
    KEY_MINUS   = 13,
    KEY_TIMES   = 14,
    KEY_DIVIDE  = 15,
    KEY_EQUAL   = 16,
    KEY_PERCENT = 17,
    KEY_CLEAR   = 18,
) key_t;

void press_key(state_t *state, key_t *key); 

/* OUTPUT */

typedef struct display_char_s {
    int has_dot;

    /* 0-9, A-Z, +, -, *, / */
    char character;
} display_char_t;

/* 0 <= index < 12, left to right. Otherwise the return value is null */
display_char_t *get_display_char(state_t *state, int index);
