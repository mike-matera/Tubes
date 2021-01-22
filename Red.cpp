/*
 * Red.cpp
 *
 */

#include "Red.h"

Red::Red() {
}

Red::~Red() {
}

int Red::render(hsv_buffer leds) {
    for (byte i = 0; i < nLEDs; i++) {
        leds[i].h = 0;
        leds[i].s = 255;
        leds[i].v = 50;
    }
    return 1000;
}

int Red::render(rgb_buffer leds) {
	CRGB c = CHSV(0,255,50);
    for (byte i = 0; i < nLEDs; i++) {
    	leds[i] = c;
    }
    return 1000;
}
