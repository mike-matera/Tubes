/*
 * Sparkle.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#include "Sparkle.h"

#define ENVELOPE "$sparkle_envelope"
#define LIMIT    "$sparkle_limit"

Sparkle::Sparkle() {
	ca = new CellularAuto(nLEDs);
	limit = 128;
	envelope = 255;
	Commands.registerVariable(LIMIT, this);
	Commands.registerVariable(ENVELOPE, this);
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

int Sparkle::render(hsv_buffer leds) {
	ca->Advance(26);
	for (byte j=0; j<nLEDs; j++) {

		int start = collected_offset[j];

		if (ca->Select(j)) {
			collected_offset[j]--;
		}else{
			collected_offset[j]++;
		}

		if (collected_offset[j] > limit) {
			collected_offset[j] = -limit;
		}

		if (collected_offset[j] < -limit) {
			collected_offset[j] = limit;
		}

		int end = collected_offset[j];

		float deriv = fabs(((float) start - (float) end) / ((float) limit * 2.0)) * 256;

		int output = deriv + ((float) leds[j].v) + ((float) envelope) * ((float) collected_offset[j]) / limit;
		if (output < 0)
			output = 0;
		else if (output > 255)
			output = 255;

		leds[j].v = output;
		leds[j].s = 255 - deriv;
	}
	return rate;
}

void Sparkle::onCommand(const std::vector<const char *> &c)
{
	Renderable::onCommand(c);
}

void Sparkle::onAssign(const char *var, const char *val)
{
	Renderable::onAssign(var, val);
	if (strncmp(LIMIT, var, CLI_LINE_MAX) == 0) {
		limit = strtoul(val, NULL, 0);
	}else if (strncmp(ENVELOPE, var, CLI_LINE_MAX) == 0) {
		envelope = strtoul(val, NULL, 0);
	}
}

void Sparkle::onReference(const char *var, char(*val)[ENVMAX])
{
	Renderable::onReference(var, val);
	if (strncmp(LIMIT, var, CLI_LINE_MAX) == 0) {
		snprintf(*val, ENVMAX, "%d", limit);
	}else if (strncmp(ENVELOPE, var, CLI_LINE_MAX) == 0) {
		snprintf(*val, ENVMAX, "%d", envelope);
	}
}
