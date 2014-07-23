/*
 * Programs.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 */

#include "Programs.h"

Programs::Programs() {
	// TODO Auto-generated constructor stub

}

Programs::~Programs() {
	// TODO Auto-generated destructor stub
}

void Programs::registerProgram(const char *name, Renderable *r)
{
	palette.push_back({name, r});
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

void Programs::render(raster ras) {
	for (std::vector<res>::iterator it = programs.begin(); it != programs.end(); it++) {
		res &r = *it;
		if (r.t <= systick_millis_count)
			r.t = r.r->render(ras) + systick_millis_count;
	}
}

Programs Progs;
