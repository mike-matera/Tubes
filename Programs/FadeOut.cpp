/*
 * FadeOut.cpp
 *
 */

#include "FadeOut.h"

FadeOut::FadeOut() {
}

FadeOut::~FadeOut() {
}

void FadeOut::setup() {
    inverseBrightness = 0;
    timer = 0;
    adjustedIncrement = increment;
}

int FadeOut::render(raster leds) {
//    if (timer >= idealElapsedTime) {
//        for (byte i = 0; i < nLEDs; i++)
//            if (leds[i].v > increment)
//                leds[i].v -= increment;
//            else
//                leds[i].v = 0;
//        timer = 0;
//    }
//
//    return 0;
    for (byte i = 0; i < nLEDs; i++) {
        if (leds[i].v >= inverseBrightness)
            leds[i].v = leds[i].v - inverseBrightness;
        else
            leds[i].v = 0;
    }

    if (timer >= idealElapsedTime) {
        adjustedIncrement = (timer / idealElapsedTime) * increment;
        if (inverseBrightness < (255 - adjustedIncrement))
            inverseBrightness += adjustedIncrement;
        else
            inverseBrightness = 255;
        timer = 0;
    }

    return 0;
}
