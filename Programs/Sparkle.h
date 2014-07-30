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
	virtual int render(hsv_buffer leds);
	virtual int render(rgb_buffer) {return 1000;};
	virtual void teardown();

	virtual void onCommand(const std::vector<const char *> &c);
	virtual void onAssign(const char *var, const char *val);
	virtual void onReference(const char *var, char(*val)[ENVMAX]);

private:
	CellularAuto *ca;
	int collected_offset[nLEDs];
	int limit;
	int envelope;
};

#endif /* SPARKLE_H_ */
