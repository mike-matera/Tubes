/*
 * Wheel.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: maximus
 */

#include "Wheel.h"

Wheel::Wheel() {
	color.h = 0;
	color.s = 255;
	color.v = 128;
	lastrun = 0;
	wheel_rate = 1000;
	Commands.registerVariable("$wheel_rate", this);
}

Wheel::~Wheel() {
}

int Wheel::render(hsv_buffer b)
{
	if ((systick_millis_count - lastrun) > wheel_rate) {
		color.h += 1;
		lastrun = systick_millis_count;
	}
	for (int i=0; i<nLEDs; i++) {
		b[i] = color;
	}
	return rate;
}

int Wheel::render(rgb_buffer b)
{
	if ((systick_millis_count - lastrun) > wheel_rate) {
		color.h += 1;
		lastrun = systick_millis_count;
	}
	CRGB c = color;
	for (int i=0; i<nLEDs; i++) {
		b[i] = c;
	}
	return rate;
}

void Wheel::onCommand(const std::vector<const char *> &c)
{
	Renderable::onCommand(c);
}

void Wheel::onAssign(const char *var, const char *val)
{
	Renderable::onAssign(var, val);
	if (strncmp("$wheel_rate", var, CLI_LINE_MAX) == 0) {
		wheel_rate = strtoul(val, NULL, 0);
	}
}

void Wheel::onReference(const char *var, char(*val)[ENVMAX])
{
	Renderable::onReference(var, val);
	if (strncmp("$wheel_rate", var, CLI_LINE_MAX) == 0) {
		snprintf(*val, ENVMAX, "%d", wheel_rate);
	}
}
