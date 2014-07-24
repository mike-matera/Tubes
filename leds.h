#ifndef _leds_h
#define _leds_h

#define nLEDs      		150
//#define USE_FAST_LED
#define USE_OCTO_LED

#include "FastLED.h"
#include "OctoWS2811.h"

typedef CHSV (&hsv_buffer)[nLEDs];
typedef CRGB (&rgb_buffer)[nLEDs];

extern CHSV HSVPixels[nLEDs];
extern CRGB RGBPixels[nLEDs];

#ifdef USE_FAST_LED
extern CRGB __fast_led_strip[];
#define led_init() FastLED.addLeds<NEOPIXEL, 2>(__fast_led_strip, nLEDs, 0)
#define led_set(n, color) __fast_led_strip[n] = color
#define led_set_rgb(n, color) __fast_led_strip[n] = color
#define led_show() FastLED.show()
#endif

#ifdef USE_OCTO_LED
extern int __drawmem[];
extern OctoWS2811 __octo_led_strip;
#define led_init() __octo_led_strip.begin()
#define led_set(n, color) {\
	CRGB p(color);\
	__octo_led_strip.setPixel(n, (((uint32_t)p.r) << 16) | (((uint32_t) p.g) << 8) | ((uint32_t)p.b)); \
	}
#define led_set_rgb(n, color) __octo_led_strip.setPixel(n, color.r, color.g, color.b);
#define led_show() __octo_led_strip.show()
#endif


#endif
