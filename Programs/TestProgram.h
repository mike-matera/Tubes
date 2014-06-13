/*
 * TestProgram.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef TESTPROGRAM_H_
#define TESTPROGRAM_H_

#include "Program.h"

class TestProgram: public Program {
public:
	TestProgram();
	virtual ~TestProgram();

public:
	virtual void render();

private:
	int led;
};

#endif /* TESTPROGRAM_H_ */
