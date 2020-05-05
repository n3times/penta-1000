#include "common.h"

#include <stdio.h>

static void test_edit() {
    test("h1", "1__       ");
    test("h12", "12_       ");
    test("h0", "___ GUESS ");
    test("h012", "12_       ");
    test("h1h", "___       ");
    test("h12h", "1__       ");
}

static void test_hi_and_lo() {
    test("h744", "744 TOO LO");
    test("h746", "746 TOO HI");
}

static void test_next_hilo_different_target() {
    test("h745", "745 YOU WON");
    test("hh745", "745 TOO HI");
}

static void test_animation_on_enter() {
    test_t *test = init_test(0);
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_reenter() {
    test_t *test = init_test(0);
    press_sequence(test, "hc");
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_clear() {
    test_t *test = init_test(0);
    press_sequence(test, "h");
    press_key(test, 'g');
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_last_guess() {
    test_t *test = init_test(0);
    press_sequence(test, "h10010010010010010010010010");
    press_key(test, '0');
    advance(test, 500);
    assert_display(test, "1 MORE GUESS");
    advance(test, 1000);
    assert_display(test, "100 TOO LO");
}

static void test_animation_on_win() {
    test_t *test = init_test(0);
    press_sequence(test, "h74");
    press_key(test, '5');
    advance(test, 10);
    assert_display(test, "745       ");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "745 YOU WON");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "1 GUESS");
}

static void test_animation_on_lose() {
    test_t *test = init_test(0);
    press_sequence(test, "h10010010010010010010010010010");
    press_key(test, '0');
    advance(test, 10);
    assert_display(test, "100       ");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "745 YOU LOST");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "745 YOU LOST");
}

static void test_show_stats() {
    test_t *test = init_test(0);
    press_sequence(test, "h555");
    assert_display(test, "555 TOO LO");
    press_key(test, '%');
    advance(test, 500);
    assert_display(test, "1 GUESS");
    advance(test, 1000);
    assert_display(test, "555 TOO LO");
}

void test_hilo2() {
    test_edit();
    test_hi_and_lo();
    test_next_hilo_different_target();
    test_animation_on_enter();
    test_animation_on_reenter();
    test_animation_on_clear();
    test_animation_on_last_guess();
    test_animation_on_win();
    test_animation_on_lose();
    test_show_stats();
}
