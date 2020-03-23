#include "penta1000.h"

typedef p1_t test_t;

void test(char *keys, char *expected);

test_t *init_test(long seed);
void press_sequence(test_t *test, char *sequence);
void press_key(test_t *test, char key);
void advance(test_t *test, long ms);
void assert_display(test_t *test, char *display);

void test_calc();
void test_game();
void test_logging();
void test_saving();
