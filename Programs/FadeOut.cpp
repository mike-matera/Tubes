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
	brightness = 255;
	lastrun = systick_millis_count;
}

int FadeOut::render(hsv_buffer leds) {
	if (brightness > 0) {
		if (systick_millis_count - lastrun > 20)
			brightness--;
	}

	for (unsigned int i=0; i<nLEDs; i++) {
		leds[i].v = (brightness < leds[i].v) ? brightness : leds[i].v;
	}

	return 0;
}

int FadeOut::render(rgb_buffer leds) {
	if (brightness > 0) {
		if (systick_millis_count - lastrun > 20)
			brightness--;
	}

	if (systick_millis_count - lastrun > 20)
		brightness--;

	for (unsigned int i=0; i<nLEDs; i++) {
		leds[i].nscale8(brightness);
	}

	return 0;
}
