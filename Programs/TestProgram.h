/*
 * TestProgram.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef TESTPROGRAM_H_
#define TESTPROGRAM_H_

#include "Renderable.h"

class TestProgram: public Renderable {
public:
	TestProgram();
	virtual ~TestProgram();

public:
	virtual int render(raster);

private:
	int led;
};

#endif /* TESTPROGRAM_H_ */
