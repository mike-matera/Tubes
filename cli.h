#ifndef _CLI_H
#define _CLI_H

#define CLI_LINE_MAX  128
#define DELIMETERS    " \r\n"

#include <vector>

#include "Stream.h"

typedef void (*command) (std::vector<const char *> &);

class CLI {

public:
	CLI(Stream &s) : dev(s) {
		pos = 0;
		echo = true;
		ss = SCANNING;
	}
	~CLI() {}

	void do_cli();
	void setEcho(bool e) {echo = e;}
	void help();
	void prompt();
	void reg(const char *cmd, const char *help, command cb);
	void exec(const char *command);

private:

	enum ttype {
		COMMAND,
		IDENT,
		VALUE
	};

	enum sstate {
		SCANNING,
		CONSUME
	};

	typedef struct {
		ttype t;
		const char *d;
	} token;

	token tok(const char *txt);
	void exec();

private:

	typedef struct {
		char *name;
		char *help;
		command cmd;
	} rgr;

	std::vector<rgr> commands;

	Stream &dev;
	char input[CLI_LINE_MAX];
	int pos;
	sstate ss;
	bool echo;
};

#endif
