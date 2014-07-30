/*
 * SpringSimulator.h
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */

#ifndef SPRINGSIMULATOR_H_
#define SPRINGSIMULATOR_H_

#include "Renderable.h"
#include "Lib/Spring.h"

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
	class Particle {
	public:
		Particle() {
			size = 0; domain = 0; offset = 0;
		};
		int render(int pixel);
		void update();

		int size, domain, offset;
		Spring s;
	};

	Particle *p1 = NULL;
	Particle *p2 = NULL;
	Particle *p3 = NULL;
};

#endif /* SPRINGSIMULATOR_H_ */
