/*
 * Sparkle.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef SPARKLE_H_
#define SPARKLE_H_

#include "Renderable.h"
#include "CellularAuto.h"
#include "ColorUtil.h"
#include "leds.h"

class Sparkle : public Renderable {
public:
	Sparkle();
	virtual ~Sparkle();

public:
	virtual void setup();
	virtual int render(raster leds);
	virtual void teardown();

private:
	CellularAuto *ca;
	int collected_offset[nLEDs];
};

#endif /* SPARKLE_H_ */
