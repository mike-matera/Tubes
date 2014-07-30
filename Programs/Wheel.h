/*
 * Wheel.h
 *
 *  Created on: Jul 29, 2014
 *      Author: maximus
 */

#ifndef WHEEL_H_
#define WHEEL_H_

#include "Renderable.h"
#include "CommandListener.h"
#include "cli.h"

class Wheel : public Renderable {
public:
	Wheel();
	virtual ~Wheel();

public:
	virtual int render(hsv_buffer);
	virtual int render(rgb_buffer);

	virtual void onCommand(const std::vector<const char *> &c);
	virtual void onAssign(const char *var, const char *val);
	virtual void onReference(const char *var, char(*val)[ENVMAX]);

private:
	CHSV color;
	unsigned int lastrun;
	unsigned int wheel_rate;
};
#endif /* WHEEL_H_ */
