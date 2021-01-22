/*
 * SpringSimulator.h
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */

#ifndef SPRINGSIMULATOR_H_
#define SPRINGSIMULATOR_H_

#include "Renderable.h"
#include "Spring.h"

class SpringSimulator : public Renderable {
public:
	SpringSimulator();
	virtual ~SpringSimulator();

public:
	virtual void setup();
	virtual int render(hsv_buffer leds);
	virtual int render(rgb_buffer);
	virtual void teardown();

private:
	Spring s0, s1, s2;
	float s1mv=0.1, s2mv=0.01;
	int lasti=0;
};

#endif /* SPRINGSIMULATOR_H_ */
