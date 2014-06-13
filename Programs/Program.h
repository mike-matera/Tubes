/*
 * Program.h
 *
 *  Created on: Jun 12, 2014
 *      Author: maximus
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <string>

class Program {
public:
	Program();
	virtual ~Program();

public:
	virtual void render() = 0;
};
#endif /* PROGRAM_H_ */
