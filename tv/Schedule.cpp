/*
 * Schedule.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#include "Schedule.h"

namespace tv {

Schedule::Schedule(Programs *progs) {
    programs = progs;
    timer = 0;
    running = false;
}

Schedule::~Schedule() {
}

void Schedule::click() {
    if (!running) {
        showIterator = shows.begin();
        nextShow();
        running = true;
    } else if (timer >= ((*showIterator)->seconds * 1000)) {
        showIterator++;

        if (showIterator != shows.end())
            nextShow();
        else
            running = false;
    }
}

void Schedule::pushShow(Show *show) {
    shows.push_back(show);
}

void Schedule::clearShows() {
	shows.clear();
}

void Schedule::nextShow() {
    if (!(*showIterator)->push)
        programs->clear();
	for (std::vector<char*>::iterator it = (*showIterator)->layers.begin(); it != (*showIterator)->layers.end(); it++)
	    programs->pushProgram(*it);
	timer = 0;
}

} /* namespace tv */
