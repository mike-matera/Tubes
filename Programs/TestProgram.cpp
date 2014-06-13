/*
 * TestProgram.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#include "TestProgram.h"
#include "leds.h"

TestProgram::TestProgram() {
	led = 0;
}

TestProgram::~TestProgram() {
	// TODO Auto-generated destructor stub
}

void TestProgram::render() {
	led_set(led, 0);
	led++;
	led = led % nLEDs;
	led_set(led, 0xffffff);
}
