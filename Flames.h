/*
 * Flames.h
 */

#ifndef FLAMES_H_
#define FLAMES_H_

#include "Renderable.h"

class Flames: public Renderable {
public:
	Flames();
	virtual ~Flames();

public:
	virtual int render(hsv_buffer);
	virtual int render(rgb_buffer);

private:
	byte maxBrightness = 200;
    unsigned int _cooling = 70;
    unsigned int _sparking = 50;
    // Array of temperature readings at each simulation cell
    uint8_t _heat[nLEDs];
    CRGB heatColor(uint8_t temperature);
};

#endif /* FLAMES_H_ */
