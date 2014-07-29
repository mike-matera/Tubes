/*
 * CommandListener.h
 *
 *  Created on: Jul 29, 2014
 *      Author: maximus
 */

#ifndef COMMANDLISTENER_H_
#define COMMANDLISTENER_H_

#include <vector>

#define ENVMAX        64

class CommandListener {
public:
	CommandListener();
	virtual ~CommandListener();

public:
	virtual void onCommand(const std::vector<const char *> &c);
	virtual void onAssign(const char *var, const char *val);
	virtual void onReference(const char *var, char(*val)[ENVMAX]);
};

#endif /* COMMANDLISTENER_H_ */
