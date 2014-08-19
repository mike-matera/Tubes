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
#include "cli.h"

namespace tv {

class Schedule : public CommandListener {
public:
    Schedule(CLI *c);
    virtual ~Schedule();
    void click();
    void push(const char *cmd);
    void clear();

	virtual void onCommand(const std::vector<const char *> &args);

private:
    CLI *shell;

    std::vector<const char *> shows;
    std::vector<const char *>::iterator showIterator;
    elapsedMillis timer;
    uint32_t sleeptime;

    bool running;
    bool stopped;
};

} /* namespace tv */

#endif /* SCHEDULE_H_ */
