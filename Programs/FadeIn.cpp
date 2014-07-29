/*
 * FadeIn.cpp
 *
 */

#include "FadeIn.h"

FadeIn::FadeIn() {
    brightness = 0;
    lastrun = 0;
}

FadeIn::~FadeIn() {
}

void FadeIn::setup() {
    brightness = 0;
    lastrun = systick_millis_count;
    inverseBrightness = 220;
    timer = 0;
    adjustedIncrement = increment;
}

int FadeIn::render(hsv_buffer leds) {
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

int FadeIn::render(rgb_buffer leds) {
	if (brightness == 255) {
		return 100000;
	}

	if (systick_millis_count - lastrun > 20)
		brightness++;

	for (unsigned int i=0; i<nLEDs; i++) {
		leds[i].nscale8(brightness);
	}

	return 0;
}
