/*
 * Environment.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 */

#include "Environment.h"
#include "Arduino.h"

Environment::Environment() {
}

Environment::~Environment() {
}

void Environment::reg(const char *var, getter g, setter s)
{
	char *v = new char[ENVMAX];
	strncpy(v, var, ENVMAX);
	env.push_back({v, g, s});
}

void Environment::set(const char *var, const char *val)
{
	for (std::vector<entry>::iterator it = env.begin(); it != env.end(); it++) {
		const char *v = (*it).var;
		if (strncmp(v, var, ENVMAX) == 0)
			(*it).s(var, val);
	}
}

const char *Environment::get(const char *var)
{
	for (std::vector<entry>::iterator it = env.begin(); it != env.end(); it++) {
		const char *v = (*it).var;
		if (strncmp(v, var, ENVMAX) == 0)
			return (*it).g(var);
	}
	return NULL;
}

Environment Env;
