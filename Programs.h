/*
 * Programs.h
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 */

#ifndef PROGRAMS_H_
#define PROGRAMS_H_

#include <vector>
#include "leds.h"
#include "Renderable.h"

class Programs {
public:
	Programs();
	virtual ~Programs();

public:
	void registerProgram(const char *name, Renderable *r);
	void pushProgram(const char *p);
	void clear();
	void render(raster r);

private:
	typedef struct {
		Renderable *r;
		uint32_t t;
	} res;

	typedef struct {
		const char *n;
		Renderable *r;
	} prog;

	std::vector<res> programs;
	std::vector<prog> palette;

};

extern Programs Progs;

#endif /* PROGRAMS_H_ */
