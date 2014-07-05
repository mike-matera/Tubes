/*
 * Sparkle.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#include "Sparkle.h"

Sparkle::Sparkle() {
	ca = new CellularAuto(nLEDs);
}

Sparkle::~Sparkle() {
	delete ca;
}

void Sparkle::setup() {
	for (int i=0; i<4; i++) {
		ca->Set(random(nLEDs));
	}
	for (int i=0; i<nLEDs; i++) {
		collected_offset[i] = 0;
	}
}

void Sparkle::teardown() {
	ca->Reset();
}

#define LIMIT      128
#define ENVELOPE   64

int Sparkle::render(raster leds) {
	ca->Advance(26);
	for (byte j=0; j<nLEDs; j++) {

		int start = collected_offset[j];

		if (collected_offset[j] > LIMIT || collected_offset[j] < -LIMIT) {
			collected_offset[j] = 0;
		}

		if (ca->Select(j)) {
			collected_offset[j]--;
		}else{
			collected_offset[j]++;
		}

		int end = collected_offset[j];
		float deriv = ((float) start - (float) end) / (float) LIMIT;

		int modulation = 256.0 * deriv;
		if (modulation < ENVELOPE)
			modulation = ENVELOPE;

		leds[j].h = (leds[j].h + ((int) (deriv * 96))) % 256;
		leds[j].v = leds[j].v + (collected_offset[j] % modulation) - modulation/2;
	}
	return 20;
}
