/*
 * Command.h
 *
 *  Created on: Jun 14, 2014
 *      Author: maximus
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#define CMDLEN   64
#define ARGSMAX  8

#include <stdint.h>
#include <Stream.h>

class Command {
public:
	Command(Stream &s) : output(s) {
		cnt=0;
		arg=0;
		crc=1;
		rcrc=0;
	}
	virtual ~Command() {

	}

public:
	void insert(const char c);

private:
	enum cstate {INIT, COMMAND, MUX, ARGS, CHKSUM};
	cstate state = INIT;

	char cmd[CMDLEN];
	char args[ARGSMAX][CMDLEN];
	int cnt, arg;
	uint16_t crc, rcrc;

	Stream &output;
};

#endif /* COMMAND_H_ */
