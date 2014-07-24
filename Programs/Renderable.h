/*
 * Renderable.h
 *
 *  Created on: Jun 15, 2014
 *      Author: maximus
 *
 *  Base class for visual programs. This is a work in progress.
 *
 *  How to use this class:
 *
 *  Constructor(s)/Destructor(s)
 *    The life cycle of a Renderable is not yet defined. Your class may come in and out of existence
 *    one or more times during a program. It's safe to allocate and dallocate memory here.
 *
 *  setup()
 *    The setup function will be run *before* this program becomes live, prior to any call to render().
 *    This can happen multiple times because programs can come and go. setup() will be run each time.
 *    This gives you the opportunity to start from whatever state you like each time.
 *
 *    setup() is optional
 *
 *  teardown()
 *    The teardown() function will be run *after* this program exits, after any call to render().
 *
 *    teardown() is optional
 *
 *  int render(raster) [pure virtual]
 *    The render() function is called for each frame of your animation. You are given a reference to
 *    an array of CHSV. See leds.h for the definition of raster. Be careful about the raster:
 *    		* Do not assume it's in the same state that it was in on your last frame. If your program
 *    		  wants total control of the animation you must write to every pixel.
 *    		* If your program wants to overlay another program you can alter the pixels you're given.
 *    		* Never store the pointer, it may change.
 *
 *    The render() function returns an integer which is interpreted by the system as the number of
 *    microseconds you want to wait before being called again. If you want to be called right away
 *    return zero. The system can only guarantee that it will be *at least* that many microseconds.
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "leds.h"

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

public:
	virtual void setup();
	virtual int render(hsv_buffer) = 0;
	virtual int render(rgb_buffer) = 0;
	virtual void teardown();
};

#endif /* RENDERABLE_H_ */
