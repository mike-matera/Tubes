/*
 * Sparkle.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef SPARKLE_H_
#define SPARKLE_H_

#include "Program.h"
#include "CellularAuto.h"
#include "ColorUtil.h"
#include "leds.h"

class Sparkle : public Program {
public:
	Sparkle();
	virtual ~Sparkle();

public:
	virtual void render();

private:
	CellularAuto *ca;
	float collected_offset[nLEDs];
	float color_pos;
};

#endif /* SPARKLE_H_ */
