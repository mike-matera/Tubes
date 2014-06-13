#ifndef _leds_h
#define _leds_h

#include "FastLED.h"
#include "OctoWS2811.h"

#define nLEDs      		150
#define USE_FAST_LED

#ifdef USE_FAST_LED
extern CRGB __led_strip[];
#define led_init() FastLED.addLeds<NEOPIXEL, 2>(__led_strip, nLEDs, 0)
#define led_set(n, color) __led_strip[n] = CRGB(color)
#define led_get(n) ((int) __led_strip[n])
#define led_show() FastLED.show()
#endif

#ifdef USE_OCTO_LED
extern int __drawmem[];
extern OctoWS2811 __led_strip;
#define led_init() __led_strip.begin()
#define led_set(n, color) __led_strip.setPixel(n, color)
#define led_get(n) __led_strip.getPixel(n)
#define led_show() __led_strip.show()
#endif


#endif
