#ifndef SPRING_H_
#define SPRING_H_

/*
 * Spring.h
 *
 *  $Id: FloatSpring.java,v 1.4 2007/06/25 23:00:10 shingoki Exp $
 *  Copyright (c) 2005-2006 shingoki
 *  Translated into C++ by maximus
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */
class Spring {

public:
	Spring(){
		springK = 1;
		dampingK = 0;
		velocity = 0;
		position = 0;
	};

	/**
	 * Make a spring with given spring constant and damping constant
	 *
	 * @param springK
	 *          Spring constant, the higher this is the "tighter" the spring, and
	 *          the more force it will exert for a given extension
	 * @param dampingK
	 *          Damping constant, the higher this is the stronger the damping, and
	 *          the more "soggy" the movement.
	 */
	Spring(float sK, float dK);

	/**
	 * Create a critically damped spring (or near to critically damped) This
	 * spring will quickly move to its target without overshooting
	 *
	 * @param springK
	 *          The spring constant - the higher this is, the more quickly the
	 *          spring will reach its target. A value of 100 gives a reasonable
	 *          response in about a second, a higher value gives a faster
	 *          response.
	 */
	Spring(float sK);

	/**
	 * Update the position of the spring. This updates the "position" as if there
	 * were a damped spring stretched between the current position and the target
	 * position. That is, the spring will tend to pull the position towards the
	 * target, and if the spring is damped the position will eventually settle
	 * onto the target.
	 *
	 * @param target
	 *          The target towards which the spring is pulling the position
	 * @param time
	 *          The elapsed time in seconds
	 */
	void update(float target, float time);

	/**
	 * @return Damping constant, the higher this is the stronger the damping, and
	 *         the more "soggy" the movement.
	 */
	float getDampingK();

	/**
	 * @param dampingK
	 *          Damping constant, the higher this is the stronger the damping, and
	 *          the more "soggy" the movement.
	 */
	void setDampingK(float dK);

	/**
	 * @return The current position of the simulated spring end point, changes as
	 *         simulation is updated
	 */
	float getPosition();

	/**
	 * @param position
	 *          A new position for simulated spring end point
	 */
	void setPosition(float p);

	/**
	 * @return The spring constant - the higher this is, the more quickly the
	 *         spring will reach its target
	 */
	float getSpringK();

	/**
	 * @param springK
	 *          The spring constant - the higher this is, the more quickly the
	 *          spring will reach its target
	 */
	void setSpringK(float sK);

	/**
	 * @return The current velocity of the position
	 */
	float getVelocity();

	/**
	 * @param velocity
	 *          A new value for the current velocity of the position
	 */
	void setVelocity(float v);

private:

	float position;
	float springK;
	float dampingK;
	float velocity;
};


#endif /* FLOATINGSPRING_H_ */
