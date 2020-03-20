#import "penta1000.h"

#import "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void assert_interval(p1_t *p1, long first, long last) {
    long actual_first;
    long actual_last;
    p1_get_log_available_interval(p1, &actual_first, &actual_last);
    printf("actual first/last: %ld/%ld\n", actual_first, actual_last);
    if (actual_first != first || actual_last != last) {
        printf("\nError!!!\n\n");
    }
}

static void assert_log(p1_t *p1, int index, char *expected) {
    char *actual = p1_get_log_entry(p1, index);
    printf("actual display: %s\n", actual);
    if (strcmp(actual, expected)) {
        printf("\nError!!! expected: %s\n\n", expected);
    }
}

void test_logging() {
    p1_t *p1 = p1_new(0);
    assert_interval(p1, 0, 0);

    press_sequence(p1, "c0/0=");
    assert_interval(p1, 1, 1);
    assert_log(p1, 1, "0/0=DIV BY ZERO");
    press_sequence(p1, "c");
    p1_clear_log(p1);

    press_sequence(p1, "1+1=");
    assert_interval(p1, 1, 1);
    assert_log(p1, 1, "1+1=2");

    for (int i = 0; i < 99; i++) {
        press_sequence(p1, "1+1=");
    }
    assert_interval(p1, 1, 100);

    press_sequence(p1, "1+1=");
    assert_interval(p1, 2, 101);

    p1_clear_log(p1);
    assert_interval(p1, 0, 0);
    press_sequence(p1,
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1=");
    assert_interval(p1, 1, 1);
    assert_log(p1, 1,
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+"
         "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1=100");
}
