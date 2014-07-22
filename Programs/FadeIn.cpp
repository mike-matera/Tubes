/*
 * FadeIn.cpp
 *
 */

#include "FadeIn.h"

FadeIn::FadeIn() {
}

FadeIn::~FadeIn() {
}

void FadeIn::setup() {
    inverseBrightness = 220;
    timer = 0;
    adjustedIncrement = increment;
}

int FadeIn::render(raster leds) {
    if (inverseBrightness >= adjustedIncrement) {
        for (byte i = 0; i < nLEDs; i++) {
            if (leds[i].v >= inverseBrightness)
                leds[i].v = leds[i].v - inverseBrightness;
            else
                leds[i].v = 0;
        }

        if (timer >= idealElapsedTime) {
            adjustedIncrement = (timer / idealElapsedTime) * increment;
            inverseBrightness -= adjustedIncrement;
            timer = 0;
        }
    }

    return 0;
}
