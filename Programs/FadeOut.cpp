/*
 * FadeOut.cpp
 *
 */

#include "FadeOut.h"

FadeOut::FadeOut() {
	brightness = 255;
	lastrun = 0;
}

FadeOut::~FadeOut() {
}

void FadeOut::setup() {
    inverseBrightness = 0;
    timer = 0;
    adjustedIncrement = increment;
}

int FadeOut::render(hsv_buffer leds) {
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

int FadeOut::render(rgb_buffer leds) {
	if (brightness > 0) {
		if (systick_millis_count - lastrun > 20)
			brightness--;
	}

	for (unsigned int i=0; i<nLEDs; i++) {
		leds[i].nscale8(brightness);
	}

	return 0;
}
