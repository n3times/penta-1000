#include "penta1000.h"

#include <curses.h>
#include <stdio.h>

static void skip_animations(p1_t *p1) {
    for (int i = 0; i < 500; i++) {
        if (!p1_is_animating(p1)) break;
        p1_advance_frame(p1);
    }
}

static void print_display(p1_t *p1) {
    char *display = p1_get_display(p1);
    printw("    [%12s]\r", display);
}

int main() {
    p1_t *p1 = p1_new(0);

    initscr();
    noecho();

    printw("Pentatronics 1000 game/calculator\n\n");
    printw("  'q' quit\n");
    printw("  'g' game\n");
    printw("  'c' clear\n");
    printw("  '~' change sign\n\n");
    skip_animations(p1);
    print_display(p1);

    while (true) {
        char pressed_key = getch();

        if (pressed_key == 'q') {
            endwin();
            break;
        }

        p1_press_key(p1, pressed_key);

        skip_animations(p1);
        print_display(p1);
    }

    return 0;
}
