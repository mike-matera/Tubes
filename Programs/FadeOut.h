/*
 * FadeOut.h
 */

#ifndef FADEOUT_H_
#define FADEOUT_H_

#include "Renderable.h"

class FadeOut: public Renderable {
public:
	FadeOut();
	virtual ~FadeOut();

public:
	virtual void setup();
	virtual int render(raster);

private:
	uint8_t inverseBrightness;
	uint8_t increment = 3;
	uint8_t adjustedIncrement;
	elapsedMillis timer;
	uint8_t idealElapsedTime = 20;
};

#endif /* FADEOUT_H_ */
