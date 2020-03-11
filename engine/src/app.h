#ifndef APP_H
#define APP_H

typedef struct app_s {
    void (*enter)(calc_t *);
    void (*press_key)(calc_t *, key_t);
    void (*advance_frame)(calc_t *);
    bool (*is_animating)(calc_t *);
} app_t;

#endif /* APP_H */
