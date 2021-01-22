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

#endif /* FADEIN_H_ */
