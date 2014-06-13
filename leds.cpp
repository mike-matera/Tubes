#include "leds.h"

#ifdef USE_FAST_LED
CRGB __led_strip[nLEDs];
#endif

#ifdef USE_OCTO_LED
int __drawmem[nLEDs*6];
OctoWS2811 __led_strip(nLEDs, __drawmem, NULL,  WS2811_GRB | WS2811_800kHz);
#endif

