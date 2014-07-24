/*
 * TestProgram.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#include "TestProgram.h"

TestProgram::TestProgram() {
	led = 0;
}

TestProgram::~TestProgram() {
	// TODO Auto-generated destructor stub
}


int TestProgram::render(hsv_buffer leds) {
	leds[led].h = 0;
	leds[led].s = 0;
	leds[led].v = 0;
	led++;
	led = led % nLEDs;
	leds[led].h = 0;
	leds[led].s = 0;
	leds[led].v = 255;
	return 10;
}

int TestProgram::render(rgb_buffer leds) {
	leds[led].r = 0;
	leds[led].g = 0;
	leds[led].b = 0;
	led++;
	led = led % nLEDs;
	leds[led].r = 255;
	leds[led].g = 255;
	leds[led].b = 255;
	return 10;
}
