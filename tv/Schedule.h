/*
 * Schedule.h
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <vector>
#include "../Programs/Programs.h"
#include "Show.h"

namespace tv {

class Schedule {
public:
    Schedule(Programs *progs);
    virtual ~Schedule();
    void click();
    void pushShow(Show *show);
    void clearShows();

private:
    void nextShow();

    Programs *programs;
    std::vector<Show*> shows;
    std::vector<Show*>::iterator showIterator;
    elapsedMillis timer;
    bool running;
};

} /* namespace tv */

#endif /* SCHEDULE_H_ */
