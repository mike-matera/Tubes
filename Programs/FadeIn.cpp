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
}

int FadeIn::render(hsv_buffer leds) {
	// Once we're done stop getting called so we don't use too much processor
	if (brightness == 255) {
		return 100000;
	}

	if (systick_millis_count - lastrun > 20)
		brightness++;

	for (unsigned int i=0; i<nLEDs; i++) {
		leds[i].v = (brightness <= leds[i].v) ? brightness : leds[i].v;
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
