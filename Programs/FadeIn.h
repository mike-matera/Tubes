/*
 * FadeIn.h
 */

#ifndef FADEIN_H_
#define FADEIN_H_

#include "Renderable.h"

class FadeIn: public Renderable {
public:
	FadeIn();
	virtual ~FadeIn();

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

#endif /* FADEIN_H_ */
