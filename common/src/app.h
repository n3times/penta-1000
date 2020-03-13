#ifndef APP_H
#define APP_H

typedef struct app_s {
    void (*enter)(p1_t *);
    void (*press_key)(p1_t *, char);
    char * (*get_display)(p1_t *);
    void (*advance_frame)(p1_t *);
    bool (*is_animating)(p1_t *);
} app_t;

#endif /* APP_H */
