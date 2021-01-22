/*
 * Command.cpp
 *
 *  Created on: Jun 14, 2014
 *      Author: maximus
 */

#include "Command.h"
#include "util/crc16.h"

void Command::insert(const char c) {
	if (state == INIT) {
		if (c == '!') {
			state = COMMAND;
			cnt = 0;
			crc = 1;
			arg = 0;
		}else{
			// XXX: Ignore? Noise on the line?
			output.print('x');
		}
	}else if (state == COMMAND) {
		if (cnt == CMDLEN) {
			output.println("protocol error: command overflow.");
			state = INIT;
		}
		crc = _crc16_update(crc, c);
		cmd[cnt] = c;
		cnt++;
		if (c == 0) {
			state = MUX;
		}
	}else if (state == MUX) {
		crc = _crc16_update(crc, c);
		if (c == '+') {
			state = ARGS;
			cnt = 0;
		}else if (c == '*') {
			state = CHKSUM;
			cnt = 0;
		}else{
			output.print("protocol error: expecting + or * but got '");
			output.print(c);
			output.println("'");
			state = INIT;
		}
	}else if (state == ARGS) {
		if (arg == ARGSMAX || cnt == CMDLEN) {
			output.println("protocol error: command or argument overflow.");
			state = INIT;
		}else{
			crc = _crc16_update(crc, c);
			args[arg][cnt] = c;
			cnt++;
			if (c == 0) {
				state = MUX;
				arg++;
			}
		}
	}else if (state == CHKSUM) {
		if (cnt == 0) {
			rcrc = 0x00ff && (uint16_t) c;
			cnt++;
		}else if (cnt == 1) {
			rcrc = 0xff00 && ((uint16_t) c) << 8;
			if (crc == rcrc) {
				// XXX: Do command
			}else{
				// XXX: Data error
			}
			state = INIT;
		}
	}
}
