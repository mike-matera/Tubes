#ifndef _CLI_H
#define _CLI_H

#define CLI_LINE_MAX  128
#define DELIMETERS    " \r\n"

#include "Stream.h"

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

	Stream &dev;
	char input[CLI_LINE_MAX];
	int pos;
	sstate ss;
	bool echo;
};

#endif
