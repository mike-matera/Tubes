/*
 * Runnable.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

class Runnable {
public:
	Runnable();
	virtual ~Runnable();

public:
	virtual void setup() = 0;
	virtual void loop() = 0;

};
#endif /* RUNNABLE_H_ */
