#ifndef APP_H
#define APP_H

#include <stdbool.h>

/* Interface that any app must implement. */

typedef struct app_s app_t;

typedef struct app_s {
    // Called when the app is about to be entered. At this point the app can do
    // any setup or can display an animation sequence for example.
    void (*enter)(app_t *);
    // The user has pressed a given key.
    void (*press_key)(app_t *, char);
    // Should return the contents of the display. This is a null terminated
    // string with up to 12 non-dot characters, any of them possibly followed by
    // a dot (such as "PI=3.14 E=2.71").
    char *(*get_display)(app_t *);
    // Called by the animation thread of the main app every 10ms if
    // 'is_animating' is true.
    void (*advance_frame)(app_t *);
    // Should return true if the app is "running", for example if the display
    // is animating. Returns false if the app is idle.
    bool (*is_animating)(app_t *);
} app_t;

#endif /* APP_H */
