/*
 * CommandListener.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: maximus
 */

#include "CommandListener.h"

CommandListener::CommandListener() {
}

CommandListener::~CommandListener() {
}

void CommandListener::onCommand(const std::vector<const char *> &c)
{
}

void CommandListener::onAssign(const char *var, const char *val)
{
}

void CommandListener::onReference(const char *var, char(*val)[ENVMAX])
{
}

