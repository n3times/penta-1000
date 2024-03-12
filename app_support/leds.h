#ifndef leds_h
#define leds_h

#include <stdint.h>

/*
 * Indicates, for each character, which segments of a 14-segment LED are on.
 * Segments top to bottom, left to right: -|\|/|--|/|\|-
 */
uint16_t app_support_get_led_segments(char c);

#endif  /* leds_h */
