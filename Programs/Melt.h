/*
 * Melt.h
 *
 *  Created on: Jun 22, 2014
 *      Author: minty
 */

#ifndef MELT_H_
#define MELT_H_

#include "Renderable.h"
#include "leds.h"
#include "Perlins.h"

class Melt : public Renderable {
public:
	Melt();
	virtual ~Melt();

public:
	virtual void setup();
	virtual int render(hsv_buffer leds);
	virtual int render(rgb_buffer) {return 1000;};
	virtual void teardown();

private:
	float step;
	float valueTracker;
	float hueTracker;
	Perlins *perlins;
};

#endif /* MELT_H_ */
