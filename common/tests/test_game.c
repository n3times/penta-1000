#include "common.h"

static void test_edit() {
    test("g1", "1__");
    test("g12", "12_");
    test("g0", "___");
    test("g012", "12_");
    test("g1c", "___");
    test("g12c", "___");
}

static void test_hilo() {
    test("g744", "TOO LOW 744");
    test("g746", "TOO HIGH 746");
}

static void test_next_game_different_target() {
    test("g745", "YOU WON 745");
    test("gc745", "TOO HIGH 745");
    test("gc293", "YOU WON 293");
}

static void test_animation_on_enter() {
    test_t *test = init_test(0);
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 1000);
    assert_display(test, "???");
    advance(test, 1000);
    assert_display(test, "___");
}

static void test_animation_on_reenter() {
    test_t *test = init_test(0);
    press_sequence(test, "gg");
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 1000);
    assert_display(test, "???");
    advance(test, 1000);
    assert_display(test, "___");
}

static void test_animation_on_clear() {
    test_t *test = init_test(0);
    press_sequence(test, "g");
    press_key(test, 'c');
    advance(test, 1000);
    assert_display(test, "???");
    advance(test, 1000);
    assert_display(test, "___");
}

static void test_animation_on_last_guess() {
    test_t *test = init_test(0);
    press_sequence(test, "g10010010010010010010010010");
    press_key(test, '0');
    advance(test, 500);
    assert_display(test, "1 MORE GUESS");
    advance(test, 500);
    assert_display(test, "TOO LOW 100");
}

static void test_animation_on_win() {
    test_t *test = init_test(0);
    press_sequence(test, "g74");
    press_key(test, '5');
    advance(test, 10);
    assert_display(test, "745");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "YOU WON 745");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "1 GUESS");
}

static void test_animation_on_lose() {
    test_t *test = init_test(0);
    press_sequence(test, "g10010010010010010010010010010");
    press_key(test, '0');
    advance(test, 10);
    assert_display(test, "100");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "YOU LOST 745");
    advance(test, 500);
    assert_display(test, "");
    advance(test, 500);
    assert_display(test, "YOU LOST 745");
}

void test_game() {
    test_edit();
    test_hilo();
    test_next_game_different_target();
    test_animation_on_enter();
    test_animation_on_reenter();
    test_animation_on_clear();
    test_animation_on_last_guess();
    test_animation_on_win();
    test_animation_on_lose();
}
