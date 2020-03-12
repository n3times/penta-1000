#include "penta1000.h"

#include <stdio.h>
#include <string.h>

static char *get_display_for_key_sequence(p1000_t *p1000, char *keys) {
    for (int i = 0; i < strlen(keys); i++) {
        p1000_press_key(p1000, keys[i]);
        while (p1000_is_animating(p1000)) {
            p1000_advance_frame(p1000);
        }
    }
    return p1000_get_display(p1000);
}

static void test(char *keys, char *expected) {
    p1000_t *p1000 = p1000_new();
    char * result = get_display_for_key_sequence(p1000, keys);
    printf("%s %s\n", keys, result);
    if (strcmp(result, expected)) {
        printf("\nError!!! result: %s expected: %s\n\n", result, expected);
    }
}

static void test_plus() {
    test("1+1=", "2");
    test("1+2+3=", "6");
    test("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1=",
         "100");
    test("1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1=",
         "OUT OF MEM");
}

static void test_minus() {
    test("5-3=", "2");
    test("10-2-3=", "5");
}

static void test_times() {
    test("2*3=", "6");
    test("2*3*4=", "24");
    test("9999999999*9999999999*9999999999*9999999999*9999999999*"
         "9999999999*9999999999*9999999999*9999999999*9999999999=",
         "OVERFLOW");
    test("9999999999*9999999999*9999999999*9999999999*9999999999*9999999999*"
         "9999999999*9999999999*9999999999*9999999999*9999999999=",
         "OVERFLOW");
}

static void test_divide() {
    test("10/2=", "5");
    test("30/2/5=", "3");
    test("1/0=", "DIV BY ZERO");
    test("0/0=", "DIV BY ZERO");
}

static void test_percent() {
    test("10%=", "10%");
    test("10%+10%=", "20%");
    test("10+10%=", "11");
    test("10+10%+10%=", "12.1");
}

int main() {
    test_plus();
    test_minus();
    test_times();
    test_divide();
    test_percent();
}
