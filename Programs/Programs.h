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
#include "cli.h"

/*
 * Programs: A class to manage the running programs. *
 */
class Programs {

public:

	enum Colorspace {HSV, RGB};

	Programs();
	~Programs();

public:

	/*
	 * Associate a name to an instance of a runnable program. This name
	 * is used by the pushProgram() function to identify which program
	 * the user wishes to activate.
	 */
	void registerProgram(const char *name, Renderable *r);

	/*
	 * Activate the named program. It must be previously registered.
	 */
	void pushProgram(const char *p);

	/*
	 * Pop the top program off of the stack.
	 */
	void popProgram();

	/*
	 * Clear the program stack.
	 */
	void clear();

	/*
	 * Call this in the main event loop to render the running programs.
	 */
	void render();

	/*
	 * Register CLI commands. Static to avoid conflicts with creating pointers
	 * to lambda functions.
	 */
	static void registerCommands(CLI &cc);

	void setColorspace(Colorspace c) {
		space = c;
	}

	Colorspace getColorspace() {
		return space;
	}

private:
	typedef struct {
		Renderable *r;
		uint32_t t;
	} res;

	typedef struct {
		const char *n;
		Renderable *r;
	} prog;

	Colorspace space;
	std::vector<res> programs;
	std::vector<prog> palette;
};

extern Programs Progs;

#endif /* PROGRAMS_H_ */
