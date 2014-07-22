/*
 * Red.cpp
 *
 */

#include "Red.h"

Red::Red() {
}

Red::~Red() {
}

int Red::render(raster leds) {
    for (byte i = 0; i < nLEDs; i++) {
        leds[i].h = 0;
        leds[i].s = 255;
        leds[i].v = 50;
    }
    return 0;
}
