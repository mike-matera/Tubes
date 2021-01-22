/*
 * Renderable.cpp
 *
 *  Created on: Jun 15, 2014
 *      Author: maximus
 */

#include "Renderable.h"

Renderable::Renderable() {
	rate = 20;
	Commands.registerVariable("$rate", this);
}

Renderable::~Renderable() {
}

void Renderable::setup() {
}

void Renderable::teardown() {
}

void Renderable::onCommand(const std::vector<const char *> &c)
{

}

void Renderable::onAssign(const char *var, const char *val)
{
	if (strncmp("$rate", var, CLI_LINE_MAX) == 0) {
		rate = strtoul(val, NULL, 0);
	}
}

void Renderable::onReference(const char *var, char(*val)[ENVMAX])
{
	if (strncmp("$rate", var, CLI_LINE_MAX) == 0) {
		snprintf(*val, ENVMAX, "%d", rate);
	}
}
