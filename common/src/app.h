#ifndef APP_H
#define APP_H

typedef struct app_s {
    void (*enter)(p1000_t *);
    void (*press_key)(p1000_t *, char);
    char * (*get_display)(p1000_t *);
    void (*advance_frame)(p1000_t *);
    bool (*is_animating)(p1000_t *);
} app_t;

#endif /* APP_H */
