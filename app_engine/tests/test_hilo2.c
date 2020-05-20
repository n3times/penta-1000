#include "common.h"

#include <stdio.h>

static void test_edit() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '1');
    assert_display(test, "1__         ");
    press_key(test, '2');
    assert_display(test, "12_         ");
    test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '0');
    advance(test, 10000);
    assert_display(test, "___ GUESS 30");
    press_key(test, '1');
    assert_display(test, "1__         ");
}

static void test_hi_and_lo() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '7');
    press_key(test, '4');
    press_key(test, '4');
    assert_display(test, "744 TOO LO  ");
    press_key(test, '7');
    press_key(test, '4');
    press_key(test, '6');
    assert_display(test, "746 TOO HI  ");
}

static void test_next_hilo_different_target() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '7');
    press_key(test, '4');
    press_key(test, '5');
    assert_display(test, "745 NICE }");
    press_key(test, 'c');
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '7');
    press_key(test, '4');
    press_key(test, '5');
    assert_display(test, "745 TOO HI  ");
}

static void test_animation_on_enter() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    assert_display(test, "> HI-LO GAME");
    advance(test, 1000);
    assert_display(test, "LEVEL 01 }");
    advance(test, 2000);
    assert_display(test, "12 GUESSES }");
    advance(test, 2000);
    assert_display(test, "30 SECONDS }");
    advance(test, 2000);
    assert_display(test, "___ GUESS 30");
}

static void test_animation_on_reenter() {
    test_t *test = init_test(0);
    press_sequence(test, "hc");
    press_key(test, 'h');
    assert_display(test, "> HI-LO GAME");
    advance(test, 1000);
    assert_display(test, "LEVEL 01 }");
    advance(test, 2000);
    assert_display(test, "12 GUESSES }");
    advance(test, 2000);
    assert_display(test, "30 SECONDS }");
    advance(test, 2000);
    assert_display(test, "___ GUESS 30");
}

static void test_animation_on_win_level() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    press_key(test, '7');
    press_key(test, '4');
    press_key(test, '5');
    advance(test, 10);
    assert_display(test, "745 NICE }");
    advance(test, 10000);
    assert_display(test, "ANY KEY");
}

static void test_show_stats() {
    test_t *test = init_test(0);
    press_key(test, 'h');
    advance(test, 10000);
    assert_display(test, "___ GUESS 30");
    press_key(test, '%');
    advance(test, 500);
    assert_display(test, "SCORE 000 }");
    advance(test, 1000);
    assert_display(test, "HI-SCORE 000");
    advance(test, 1000);
    assert_display(test, "___ GUESS 30");
}

void test_hilo2() {
    test_edit();
    test_hi_and_lo();
    test_next_hilo_different_target();
    test_animation_on_enter();
    test_animation_on_reenter();
    test_animation_on_win_level();
    test_show_stats();
}
