/*
 * SequenceTest.cpp
 *
 */

#include "SequenceTest.h"

SequenceTest::SequenceTest() {
}

SequenceTest::~SequenceTest() {
}

int SequenceTest::render(raster leds) {
    for (byte i = 0; i < nLEDs; i++) {
        leds[i].h = 0;
        leds[i].s = 255;
        leds[i].v = 50;
    }
    return 0;
}
