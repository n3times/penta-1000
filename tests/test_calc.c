#include "penta1000.h"

#include <stdio.h>
#include <string.h>

static char *get_display_for_key_sequence(p1000_t *p1000, char *keys) {
    for (int i = 0; i < strlen(keys); i++) {
        do {
            p1000_press_key(p1000, keys[i]);
        } while(p1000_is_animating(p1000));
    }
    return p1000_get_display(p1000);
}

static void test(char *keys, char *expect) {
    p1000_t *p1000 = p1000_new();
    char * result = get_display_for_key_sequence(p1000, keys);
    printf("%s %s\n", keys, result);
    if (strcmp(result, expect)) {
        printf("Error!!!\n");
    }
}

int main() {
    test("1+1=", "2");
    test("1+2+3=", "6");
    test("5-3=", "2");
    test("10-2-3=", "5");
    test("2*3=", "6");
    test("2*3*4=", "24");
    test("10/2=", "5");
    test("30/2/5=", "3");
    test("10+10%=", "11");
    test("10+10%+10%=", "12.1");
}
