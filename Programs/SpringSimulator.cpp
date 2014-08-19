/*
 * SpringSimulator.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */

#include <math.h>

#include "SpringSimulator.h"

#define SDOMAIN (nLEDs)

SpringSimulator::SpringSimulator() {
	// TODO Auto-generated constructor stub

}

SpringSimulator::~SpringSimulator() {
	// TODO Auto-generated destructor stub
}

void SpringSimulator::setup()
{
	int energy = random(128) + 127;
	int cog = random(256);

	s0.setPosition(energy);
	s0.setSpringK(0.0000001);
	s0.setDampingK(0.0);
	s0.setVelocity(0);

	s1.setPosition(-2*cog/3);
	s1.setSpringK(0.000001);
	s1.setDampingK(0.00);
	s1.setVelocity(0);

	s2.setPosition(3*cog/5);
	s2.setSpringK(0.0000001);
	s2.setDampingK(0.00);
	s2.setVelocity(0);
}

int SpringSimulator::render(hsv_buffer leds)
{
	s0.update(0,1);
	s1.update(s0.getPosition(),1);
	s2.update(s1.getPosition(),1);

	float v = s1.getVelocity();
	if (v > s1mv)
		s1mv = v;
	float s1scale = abs(v) * (8 / s1mv) + 1;

	v = s2.getVelocity();
	if (v > s2mv)
		s2mv = v;
	float s2scale = abs(v) * (8 / s2mv) + 1 ;

	lasti=0;
	for (int i=0; i<nLEDs; i++) {
		int pos = nLEDs/2 - i;

		float offset = (512 / (s1scale + 4));
		float gd = (pos - s1.getPosition()) / offset;
		gd = (gd - floor(gd)) * offset;
		gd = abs(gd - (offset/2));
		gd = gd * s1scale;
		if (gd > 255)
			gd = 255;

		offset = (1024 / (s2scale + 4));
		float bd = (pos - s2.getPosition()) / offset;
		bd = (bd - floor(bd)) * offset;
		bd = abs(bd - (offset/2));
		bd = bd * s2scale;
		if (bd > 255)
			bd = 255;

		float hh;
		if (gd == 0 && bd == 0)
			hh = 0.5;
		else
			hh = bd / (gd + bd);

		hh = 106 + (32 * hh);
		leds[i].h = hh;
		leds[i].s = 255;

		uint8_t value = ((255 - gd) + (255 - bd)) / 2;
		lasti+= value;
		leds[i].v = value;
	}


	return rate;
}

int SpringSimulator::render(rgb_buffer leds)
{
	s0.update(0,1);
	s1.update(s0.getPosition(),1);
	s2.update(s1.getPosition(),1);

	for (int i=0; i<nLEDs; i++) {
		int pos = nLEDs/2 - i;
		int rd = abs(pos - s0.getPosition()) * (abs(s0.getVelocity()) * 64);
		if (rd> 255)
			rd = 255;

		int gd = abs(pos - s1.getPosition()) * (abs(s1.getVelocity()) * 64);
		if (gd> 255)
			gd = 255;

		int bd = abs(pos - s2.getPosition()) * (abs(s2.getVelocity()) * 64);
		if (bd> 255)
			bd = 255;

		//leds[i].h = abs(s1.getPosition() - s2.getPosition()/128);
		//leds[i].r = (255 - rd);
		leds[i].g = (255 - gd);
		leds[i].r = 0;
		leds[i].b = (255 - bd);
	}

	return rate;
}

void SpringSimulator::teardown()
{

}
