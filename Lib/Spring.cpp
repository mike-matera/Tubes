/*
 * Spring.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: maximus
 */

#include "Spring.h"
#include <cmath>

Spring::Spring(float sK, float dK) {
	position = 0;
	springK = sK;
	dampingK = dK;
	velocity = 0;
}

Spring::Spring(float sK) : Spring(sK, 2.0 * std::sqrt(sK)) {
}

void Spring::update(float target, float time) {

	// Set v to target - position, this is the required movement
	float v = position - target;

	// Multiply displacement by spring constant to get spring force,
	// then subtract damping force
	v = v * -springK - velocity * dampingK;

	// v is now a force, so assuming unit mass is is also acceleration.
	// multiply by elapsed time to get velocity change
	velocity += v * time;

	// If velocity isn't valid, zero it
	if (std::isnan(velocity) || std::isinf(velocity)) {
		velocity = 0;
	}

	// Change the position at the new velocity, for elapsed time
	position += velocity * time;
}

float Spring::getDampingK() {
	return dampingK;
}

void Spring::setDampingK(float dK) {
	dampingK = dK;
}

float Spring::getPosition() {
	return position;
}

void Spring::setPosition(float p) {
	position = p;
}

float Spring::getSpringK() {
	return springK;
}

void Spring::setSpringK(float sK) {
	springK = sK;
}

float Spring::getVelocity() {
	return velocity;
}

void Spring::setVelocity(float v) {
	velocity = v;
}
