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
};

#endif /* FADEOUT_H_ */
