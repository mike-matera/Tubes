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
};

#endif /* FADEIN_H_ */
