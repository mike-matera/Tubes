/*
 * Melt.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: minty
 */

#include "Melt.h"

Melt::Melt() {
    random16_add_entropy(random());
	valueTracker = random16();
    random16_add_entropy(random());
	hueTracker = random16();
	perlins = new Perlins();
}

Melt::~Melt() {
    delete perlins;
}

void Melt::setup() {
	// <minty> just continue where we left off, no need to reset
	step = -1;
}

void Melt::teardown() {
}

int Melt::render(hsv_buffer leds) {
	float value, hue;

	step += 0.0005;
	if (step > 3.141) {
		step = -3.141;
	}

	// Base rate is 0.0001;
	float rate = 0.001 + (0.001 * (sin(step) + 1));

    valueTracker += rate * 10.0;
    hueTracker += rate;

    for (int i = 0; i < nLEDs; i++)
    {
        valueTracker += 0.0001;
        hueTracker += 0.00001;

        value = perlins->pnoise(valueTracker + sin((i + valueTracker) / 2) , cos(valueTracker), valueTracker);
        hue = perlins->pnoise(cos(hueTracker / 5.0) + sin((i + hueTracker) / 30.0) , cos(hueTracker / 5.0), hueTracker);

        // <minty> adjust to 1 - 255
        leds[i].h = (hue * 127) + 128;
        // <minty> adjust to 75 - 225
        //leds[i].h = ((hue + (double)2) * 75);
        leds[i].s = 255;
        leds[i].v = (value * (double) 127) + 128;
    }

    return 0;
}
