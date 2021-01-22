/*
 * Perlins.h
 *
 *  Created on: Jun 22, 2014
 *      Author: minty
 */

#ifndef PERLINS_H_
#define PERLINS_H_

#define P(x) p[(x) & 255]

class Perlins {
public:
	Perlins();
	virtual ~Perlins();
	double pnoise(double x, double y, double z);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

#endif /* PERLINS_H_ */
