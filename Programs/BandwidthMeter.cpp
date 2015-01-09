/*
 * BandwidthMeter.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: maximus
 */

#include "BandwidthMeter.h"
#include "cli.h"

BandwidthMeter::BandwidthMeter() {
	err.setPosition(60);
	err.setSpringK(0.01);
	err.setDampingK(0.0);
	err.setVelocity(0);

	for (int i=0; i<nLEDs; i++) {
		nextvals[i] = 0;
		oldvals[i] = 0;
	}
}

BandwidthMeter::~BandwidthMeter() {
}

void BandwidthMeter::setup() {
	Commands.registerCommand("sample", "Insert a color sample into the history", this);
}

int BandwidthMeter::render(hsv_buffer leds)
{
	for (int i=0; i<nLEDs; i++) {
		leds[i].v = 0;
	}
	return 1000;
}

int BandwidthMeter::render(rgb_buffer leds)
{
	if ((lastud + DEADLINE) < systick_millis_count) {
		// We're not receiving updates! Go to emergency mode.
		err.update(75,1);
		float sp = err.getPosition();
		for (int i=0; i<nLEDs; i++) {
			float gd = abs((i - sp) * 32);
			if (gd > 255)
				gd = 255;
			gd = 255 - gd;

			leds[i].r = gd;
			leds[i].g = 0;
			leds[i].b = 0;
		}
		frame = 0;
		return 20;
	}

	int fps = (1000 / rate);
	if (frame == 0) {
		samples[pos++] = last;
		pos = pos % SAMPLES;
		for (int i=0; i<nLEDs; i++) {
			sample sum;
			int led = nLEDs-i-1;
			for (int j=0; j<AVG; j++) {
				sum += samples[(pos + (i*AVG + j)) % SAMPLES];
			}
			sum /= AVG;
			oldvals[led] = nextvals[led];
			nextvals[led] = sum.toRGB();
		}
	}
	float step = ((float)frame + 1.0) / (float)fps;
	for (int i=0; i<nLEDs; i++) {
		int led = nLEDs-i-1;
		float r = (float)oldvals[led].r + (((float)nextvals[led].r - (float)oldvals[led].r) * step);
		float g = (float)oldvals[led].g + (((float)nextvals[led].g - (float)oldvals[led].g) * step);
		float b = (float)oldvals[led].b + (((float)nextvals[led].b - (float)oldvals[led].b) * step);
		leds[nLEDs-i-1] = CRGB(r, g, b);
	}
	frame = (frame + 1) % fps;
	return rate;
}

void BandwidthMeter::teardown()
{
}

void BandwidthMeter::onCommand(const std::vector<const char *> &args)
{
	if (strncmp("sample", args[0], CLI_LINE_MAX) == 0 && args.size() == 4) {
		sample s;
		s.r = strtof(args[1],NULL);
		s.g = strtof(args[2],NULL);
		s.b = strtof(args[3],NULL);
		/*
		samples[pos++] = s;
		pos = pos % SAMPLES;
		*/
		last = s;
		lastud = systick_millis_count;
	}else{
		Renderable::onCommand(args);
	}
}


void BandwidthMeter::onAssign(const char *var, const char *val)
{
	Renderable::onAssign(var, val);
}

void BandwidthMeter::onReference(const char *var, char(*val)[ENVMAX])
{
	Renderable::onReference(var, val);
}
