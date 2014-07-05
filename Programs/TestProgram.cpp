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

int TestProgram::render(raster leds) {
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
