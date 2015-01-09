/*
 * BandwidthMeter.h
 *
 *  Created on: Jan 8, 2015
 *      Author: maximus
 */

#ifndef BANDWIDTHMETER_H_
#define BANDWIDTHMETER_H_

#include "Renderable.h"
#include "Lib/Spring.h"

#define SAMPLES     1800
#define DEADLINE    10000
#define AVG         (SAMPLES/nLEDs)

class sample {
public:
	sample() : r(0), g(0), b(0) {
	}

	sample(const sample& other) {
		r = other.r;
		g = other.g;
		b = other.b;
	}

	sample &operator+=(const sample &other) {
		r += other.r;
		g += other.g;
		b += other.b;
		return *this;
	}

	sample &operator/=(const float scale) {
		r /= scale;
		g /= scale;
		b /= scale;
		return *this;
	}

	const CRGB toRGB() {
		return CRGB(r, g, b);
	}

	float r,g,b;
};

class BandwidthMeter: public Renderable {
public:
	BandwidthMeter();
	virtual ~BandwidthMeter();

public:
	virtual void setup();
	virtual int render(hsv_buffer leds);
	virtual int render(rgb_buffer);
	virtual void teardown();

	virtual void onCommand(const std::vector<const char *> &c);
	virtual void onAssign(const char *var, const char *val);
	virtual void onReference(const char *var, char(*val)[ENVMAX]);

private:

	int pos = 0;
	int frame = 0;
	unsigned int lastud = 0;
	sample samples[SAMPLES];
	CRGB nextvals[nLEDs];
	CRGB oldvals[nLEDs];
	sample last;

	Spring err;
};

#endif /* BANDWIDTHMETER_H_ */
