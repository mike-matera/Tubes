/*
 * Environment.h
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <vector>
#include <stdint.h>

#define ENVMAX 64

typedef void (*getter) (const char *, char(*)[ENVMAX]);
typedef void (*setter) (const char *, const char *);

class Environment {
public:
	Environment();
	virtual ~Environment();

	void reg(const char *var, getter g, setter s);
	void set(const char *var, const char *val);
	const char *get(const char *var);

private:
	typedef struct {
		char *var;
		getter g;
		setter s;
	} entry;

	std::vector<entry> env;
	char buffer[ENVMAX];
};

extern Environment Env;

#endif /* ENVIRONMENT_H_ */
