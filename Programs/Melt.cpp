/*
 * Melt.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: minty
 */

#include "Melt.h"

Melt::Melt() {
	valueTracker = random16();
	hueTracker = random16();
	perlins = new Perlins();
}

Melt::~Melt() {
    delete perlins;
}

void Melt::setup() {
	// <minty> just continue where we left off, no need to reset
}

void Melt::teardown() {
}

int Melt::render(raster leds) {
	float value, hue;

    valueTracker += 0.001;
    hueTracker += 0.0001;

    for (int i = 0; i < nLEDs; i++)
    {
        valueTracker += 0.0001;
        hueTracker += 0.00001;

        value = perlins->pnoise(valueTracker + sin((i + valueTracker) / 2) , cos(valueTracker), valueTracker);
        hue = perlins->pnoise(cos(hueTracker / 5.0) + sin((i + hueTracker) / 30.0) , cos(hueTracker / 5.0), hueTracker);

        // <minty> adjust to 1 - 255
        leds[i] = CHSV(((hue * 127) + 128), 255, ((value * (double) 127) + 128));
    }

    return 0;
}
