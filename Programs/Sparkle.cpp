/*
 * Sparkle.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#include "Sparkle.h"

Sparkle::Sparkle() {
	ca = new CellularAuto(nLEDs);
	color_pos = 0;
}

Sparkle::~Sparkle() {
	delete ca;
}

void Sparkle::setup() {
	color_pos = ((float) random(256)) / (256.0 / 6.0);
	ca->Set(random(32));
	ca->Set(random(32));
	for (int i=random(3); i>0; i--) {
		ca->Set(random(32));
	}
}

void Sparkle::teardown() {
	ca->Reset();
}

int Sparkle::render(raster leds) {
	if (color_pos < 6) {
		color_pos += 0.0001;
	}else{
		color_pos = 0;
	}
	ca->Advance(26);
	for (byte j=0; j<nLEDs; j++) {
		if (collected_offset[j] > 100 || collected_offset[j] < -100) {
			collected_offset[j] = 0;
		}
		float start = floor(collected_offset[j]);

		if (ca->Select(j)) {
			collected_offset[j] += -0.01;
		}else{
			collected_offset[j] -= -0.01;
		}
		float end = floor(collected_offset[j]);
		float deriv = start - end;

		float value = fmod(collected_offset[j],1);
		if (value > 0.25 && deriv < 0.9) value = 0;

		leds[j].h = ((color_pos+(0.001*j)+deriv*3) / 6) * 255;
		leds[j].s = 255;
		leds[j].v = value * 255;
	}
	return 33;
}
