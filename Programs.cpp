/*
 * Programs.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 */

#include "Programs.h"
#include "leds.h"
#include "nvram.h"

Programs::Programs() {
	space = HSV;
}

Programs::~Programs() {
}

void Programs::registerProgram(const char *name, Renderable *r)
{
	palette.push_back({name, r});
	Serial.printf("Registering program: \"%s\"\r\n", name);
}

void Programs::popProgram()
{
	if (programs.size() == 0)
		return;

	programs.back().r->teardown();
	programs.pop_back();
}

void Programs::pushProgram(const char *name)
{
	// Find the program in the pallete
	for (std::vector<prog>::iterator it = palette.begin(); it != palette.end(); it++) {
		if (strcmp(name, (*it).n) == 0) {
			(*it).r->setup();
			programs.push_back({(*it).r, 0});
			return;
		}
	}
}

void Programs::clear() {
	for (std::vector<res>::iterator it = programs.begin(); it != programs.end(); it++) {
		(*it).r->teardown();
	}
	programs.clear();
}

void Programs::render() {
	if (programs.size() == 0)
		return;

	res &r = programs[0];
	if (r.t <= systick_millis_count) {
		if (space == HSV) {
			r.t = r.r->render(HSVPixels) + systick_millis_count;
		}else{
			r.t = r.r->render(RGBPixels) + systick_millis_count;
		}
		// The first program asked to render... do the rest.
		for (unsigned int i=1; i<programs.size(); i++) {
			res &rr = programs[i];
			if (space == HSV) {
				rr.r->render(HSVPixels);
			}else{
				rr.r->render(RGBPixels);
			}
		}
	}

	// Render the HSV buffer onto our RGB pixels
	if (space == HSV) {
		for (int i=0; i<nLEDs; i++) {
			led_set(i, HSVPixels[i]);
		}
	}else{
		for (int i=0; i<nLEDs; i++) {
			led_set_rgb(i, RGBPixels[i]);
		}
	}
	led_show();
}

void Programs::registerCommands()
{
	Commands.registerCommand("program", "program [<p1>...<pN>] -- Set the program stack.", this);
	Commands.registerCommand("push", "push [<p1> [...] -- Push the programs onto the stack.", this);
	Commands.registerCommand("pop", "pop -- Pop the last program off of the stack.", this);
	Commands.registerCommand("colorspace", "colorspace [hsv|rgb] -- Change the program colorspace.", this);
	Commands.registerVariable("$name", this);
}

// Command Listener
void Programs::onCommand(const std::vector<const char *> &ps)
{
	if (strncmp("program", ps[0], CLI_LINE_MAX) == 0) {
		Progs.clear();
		for (unsigned int i=1; i<ps.size(); i++) {
			Progs.pushProgram(ps[i]);
		}
	}else if (strncmp("push", ps[0], CLI_LINE_MAX) == 0) {
		for (unsigned int i=1; i<ps.size(); i++) {
			Progs.pushProgram(ps[i]);
		}
	}else if (strncmp("pop", ps[0], CLI_LINE_MAX) == 0) {
		Progs.popProgram();
	}else if (strncmp("colorspace", ps[0], CLI_LINE_MAX) == 0) {
		if (ps.size() < 2)
			return;
		if (strcmp("hsv", ps[1]) == 0) {
			Progs.setColorspace(HSV);
		} else if (strcmp("rgb", ps[1]) == 0) {
			Progs.setColorspace(RGB);
		}
	}
}

void Programs::onAssign(const char *var, const char *val)
{
	strncpy(nvram.xbee_name, val, sizeof(nvram.xbee_name));
	store_nvram();
}

void Programs::onReference(const char *var, char(*val)[ENVMAX])
{
	snprintf(*val, sizeof(nvram.xbee_name), "%s", nvram.xbee_name);
}


Programs Progs;
