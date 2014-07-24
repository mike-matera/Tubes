/*
 * Environment.h
 *
 *  Created on: Jul 22, 2014
 *      Author: maximus
 *
 *  Interface to manage the runtime environment of the application.
 *  Instead of using memory based variables this class binds variable
 *  names to getter and setter callbacks. This allows observers to
 *  perform actions when a variable is read or altered.
 *
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <vector>
#include <stdint.h>

#define ENVMAX 64

/*
 * Prototypes for getter and setter.
 */
typedef void (*getter) (const char *, char(*)[ENVMAX]);
typedef void (*setter) (const char *, const char *);

class Environment {
public:
	Environment();
	virtual ~Environment();

	/*
	 * Register a callbacks for the variable. Variables MUST start with a
	 * dollar sign ($). They won't be called otherwise.
	 */
	void reg(const char *var, getter g, setter s);

	/*
	 * Call all setters registered to listen to var. It's possible for a
	 * setter to listen on multiple variables so the name of the referenced
	 * variable is passed
	 */
	void set(const char *var, const char *val);

	/*
	 * Call the first getter registered for this variable. The value
	 * returned by the first getter will be returned. Memory returned
	 * is volatile and may not persist after the next call to get()
	 */
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
