/*
 * SequenceTest.h
 */

#ifndef SEQUENCETEST_H_
#define SEQUENCETEST_H_

#include "Renderable.h"

class SequenceTest: public Renderable {
public:
	SequenceTest();
	virtual ~SequenceTest();

public:
	virtual int render(hsv_buffer);
	virtual int render(rgb_buffer);

private:
};

#endif /* SEQUENCETEST_H_ */
