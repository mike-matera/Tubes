/*
 * Schedule.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#include "Schedule.h"

namespace tv {

Schedule::Schedule(CLI *c) {
    timer = 0;
    sleeptime = 0;
    running = false;
    shell = c;
    stopped = false;
    Commands.registerCommand("sleep", "sleep <seconds> -- Tell the scheduler to do nothing for a while", this);
    Commands.registerCommand("next", "next -- Tell the scheduler to move on (even if it's sleeping)", this);
    Commands.registerCommand("stop", "stop -- Stop the scheduler", this);
    Commands.registerCommand("start", "start -- Start the scheduler", this);
}

Schedule::~Schedule() {
}

void Schedule::onCommand(const std::vector<const char *> &args) {
	if (strncmp("sleep", args[0], CLI_LINE_MAX) == 0) {
		sleeptime = strtoul(args[1],NULL,0);
		timer = 0;
	}else if (strncmp("next", args[0], CLI_LINE_MAX) == 0) {
		sleeptime = 0;
	}else if (strncmp("start", args[0], CLI_LINE_MAX) == 0) {
		stopped = false;
	}else if (strncmp("stop", args[0], CLI_LINE_MAX) == 0) {
		stopped = true;
	}
}

void Schedule::click() {
	if (stopped)
		return;

    if (!running) {
        showIterator = shows.begin();
        Serial.printf("Schedule: %s\r\n", *showIterator);
    	shell->exec(*showIterator);
        running = true;
    } else if (timer >= (sleeptime * 1000)) {
        showIterator++;

        if (showIterator != shows.end()) {
            Serial.printf("Schedule: %s\r\n", *showIterator);
        	shell->exec(*showIterator);
        } else
            running = false;
    }
}

void Schedule::push(const char *command) {
    shows.push_back(command);
}

void Schedule::clear() {
	shows.clear();
}

} /* namespace tv */
