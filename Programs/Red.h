/*
 * Red.h
 */

#ifndef RED_H_
#define RED_H_

#include "Renderable.h"

class Red: public Renderable {
public:
	Red();
	virtual ~Red();

public:
	virtual int render(hsv_buffer);
	virtual int render(rgb_buffer);

private:
};

#endif /* RED_H_ */
