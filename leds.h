#ifndef _leds_h
#define _leds_h

#define nLEDs      		150
//#define USE_FAST_LED
#define USE_OCTO_LED

#include "FastLED.h"
#include "OctoWS2811.h"

typedef CHSV (&raster)[nLEDs];

#ifdef USE_FAST_LED
extern CRGB __led_strip[];
#define led_init() FastLED.addLeds<NEOPIXEL, 2>(__led_strip, nLEDs, 0)
#define led_set(n, color) __led_strip[n] = color
#define led_show() FastLED.show()
#endif

#ifdef USE_OCTO_LED
extern int __drawmem[];
extern OctoWS2811 __led_strip;
#define led_init() __led_strip.begin()
#define led_set(n, color) {\
	CRGB p(color);\
	__led_strip.setPixel(n, (((uint32_t)p.r) << 16) | (((uint32_t) p.g) << 8) | ((uint32_t)p.b)); \
	}
#define led_show() __led_strip.show()
#endif


#endif
