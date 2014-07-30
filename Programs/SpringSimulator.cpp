/*
 * SpringSimulator.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */

#include "SpringSimulator.h"

SpringSimulator::SpringSimulator() {
	// TODO Auto-generated constructor stub

}

SpringSimulator::~SpringSimulator() {
	// TODO Auto-generated destructor stub
}

void SpringSimulator::setup()
{
	if (p1 == NULL) {
		p1 = new Particle();
		p2 = new Particle();
		p3 = new Particle();
	}
	p1->size = random(32-4) + 4;
	p1->domain = nLEDs + 2.0 * (float) nLEDs * ((float) random(100) / 100);
	p1->offset = (random(p1->domain)/2);
	p1->s.setPosition(random(p2->domain)/2);
	p1->s.setSpringK(0.00001);
	p1->s.setDampingK(0.0);
	p1->s.setVelocity(0);

	p2->size = random(32-4) + 4;
	p2->domain = nLEDs + 2.0 * (float) nLEDs * ((float) random(100) / 100);
	p2->offset = (random(p2->domain)/2);
	p2->s.setPosition(random(p2->domain)/2);
	p2->s.setSpringK(0.00001);
	p2->s.setDampingK(0.0);
	p2->s.setVelocity(0);

	p3->size = random(32-4) + 4;
	p3->domain = nLEDs + 2.0 * (float) nLEDs * ((float) random(100) / 100);
	p3->offset = (random(p3->domain)/2);
	p3->s.setPosition(random(p3->domain)/2);
	p3->s.setSpringK(0.00001);
	p3->s.setDampingK(0.0);
	p3->s.setVelocity(0);

	Serial.printf("DEBUG: s: %d o: %d d: %d\r\n", p1->size, p1->offset, p1->domain);
}

int SpringSimulator::render(hsv_buffer leds)
{
	return rate;
}

int SpringSimulator::render(rgb_buffer leds)
{
	for (int i=0; i<nLEDs; i++) {
		leds[i].r = p1->render(i);
		leds[i].g = p2->render(i);
		leds[i].b = p3->render(i);
	}
	p1->update();
	p2->update();
	p3->update();
	return rate;
}

void SpringSimulator::teardown()
{

}

int SpringSimulator::Particle::render(int pixel)
{
	int o = size * abs(pixel - (s.getPosition() - offset));
	if (o > 255)
		o = 255;
	return 255 - o;
}

void SpringSimulator::Particle::update()
{
	s.update(domain/2,1);
	//Serial.printf("DEBUG: pos: %f\r\n", s.getPosition());
}
