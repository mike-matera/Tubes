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
	virtual int render(hsv_buffer);
	virtual int render(rgb_buffer);

private:
	uint8_t brightness;
	uint32_t lastrun;
    uint8_t inverseBrightness;
    uint8_t increment = 2;
    uint8_t adjustedIncrement;
    elapsedMillis timer;
    uint8_t idealElapsedTime = 30;
};

#endif /* FADEOUT_H_ */
