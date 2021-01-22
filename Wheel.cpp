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
	color.v = 0;
	lastrun = 0;
	wheel_rate = 1000;
	wheel_origin = 0;
	wheel_range = 255;
	wheel_up = true;
	Commands.registerVariable("$wheel_rate", this);
	Commands.registerVariable("$wheel_origin", this);
	Commands.registerVariable("$wheel_range", this);
	Commands.registerVariable("$wheel_value", this);
}

Wheel::~Wheel() {
}

void Wheel::setup()
{
	color.h = wheel_origin;
	color.v = wheel_value;
	color.s = 255;
}

int Wheel::render(hsv_buffer b)
{
	if ((systick_millis_count - lastrun) > wheel_rate) {
		lastrun = systick_millis_count;
		update();
	}
	for (int i=0; i<nLEDs; i++) {
		b[i] = color;
	}
	return rate;
}

int Wheel::render(rgb_buffer b)
{
	if ((systick_millis_count - lastrun) > wheel_rate) {
		lastrun = systick_millis_count;
		update();
	}
	CRGB c = color;
	for (int i=0; i<nLEDs; i++) {
		b[i] = c;
	}
	return rate;
}

void Wheel::update()
{
	if (wheel_range == 255) {
		color.h += 1;
		return;
	}

	if (wheel_up) {
		uint8_t target = wheel_origin + wheel_range;
		color.h++;
		if (color.h == target)
			wheel_up = false;
	}else{
		color.h--;
		if (color.h == wheel_origin)
			wheel_up = true;
	}
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
	}else if (strncmp("$wheel_origin", var, CLI_LINE_MAX) == 0) {
		wheel_origin = strtoul(val, NULL, 0);
	}else if (strncmp("$wheel_range", var, CLI_LINE_MAX) == 0) {
		wheel_range = strtoul(val, NULL, 0);
	}else if (strncmp("$wheel_value", var, CLI_LINE_MAX) == 0) {
		wheel_value = strtoul(val, NULL, 0);
	}
}

void Wheel::onReference(const char *var, char(*val)[ENVMAX])
{
	Renderable::onReference(var, val);
	if (strncmp("$wheel_rate", var, CLI_LINE_MAX) == 0) {
		snprintf(*val, ENVMAX, "%d", wheel_rate);
	}
}
