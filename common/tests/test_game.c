#include "common.h"

static void test_start() {
    test("g", "___");
}

static void test_low() {
    test("g744", "TOO LOW 744");
}

static void test_high() {
    test("g746", "TOO HIGH 746");
}

static void test_reset_on_clear() {
    test("g746c", "___");
}

static void test_win() {
    test("g745", "YOU WON 745");
}

static void test_lose() {
    test("g100100100100100100100100100100", "YOU LOST 745");
}

static void test_near_lose() {
    test("g100100100100100100100100100", "TOO LOW 100");
}

static void test_edit_initial_zero() {
    test("g0", "___");
}

static void test_edit() {
    test("g12", "12_");
}

static void test_next_game_next_guess() {
    test("gc745", "TOO HIGH 745");
    test("gc293", "YOU WON 293");
}

static void test_restart_on_reenter() {
    test("g12gg", "___");
    test("g12gg293", "YOU WON 293");
}

void test_game() {
    test_start();
    test_edit();
    test_edit_initial_zero();
    test_low();
    test_high();
    test_win();
    test_near_lose();
    test_lose();
    test_reset_on_clear();
    test_next_game_next_guess();
    test_restart_on_reenter();
}
