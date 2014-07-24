/*
 * SequenceTest.cpp
 *
 */

#include "SequenceTest.h"

SequenceTest::SequenceTest() {
}

SequenceTest::~SequenceTest() {
}

int render(hsv_buffer leds) {
    for (byte i = 0; i < nLEDs; i++) {
        leds[i].h = 0;
        leds[i].s = 255;
        leds[i].v = 50;
    }
    return 0;
}

int render(rgb_buffer) {
    return 10000;
}

