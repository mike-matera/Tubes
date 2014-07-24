#ifndef _CLI_H
#define _CLI_H

#define CLI_LINE_MAX  128
#define DELIMETERS    " \r\n"

#include <vector>

#include "Stream.h"

/*
 * Prototype for a command callback. The vector contains the tokens
 * on the line that was entered by the user including the name of your
 * command. That way one callback can handle multiple commands.
 *
 * arg[0]: Name of the command
 * arg[1]: First argument
 * ...
 */
typedef void (*command) (std::vector<const char *> &);

class CLI {

public:
	CLI(Stream &s) : dev(s) {
		pos = 0;
		echo = true;
		ss = SCANNING;
	}
	~CLI() {}

	/*
	 * Consume characters from our device if available, possibly running
	 * a command. This function will not block.
	 */
	void do_cli();

	/*
	 * Allow printing of the prompt and responses.
	 */
	void setEcho(bool e) {echo = e;}

	/*
	 * Print the help
	 */
	void help();

	/*
	 * Print the prompt
	 */
	void prompt();

	/*
	 * Register a command callback.
	 *
	 * cmd: The command as it will appear on the command line
	 * help: The help message that's printed when the user types 'help'
	 * cb: The command callback.
	 */
	void reg(const char *cmd, const char *help, command cb);

	/*
	 * Execute the string as if it were typed on the command line.
	 */
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

	int pos;
	sstate ss;
	bool echo;
	Stream &dev;
	char input[CLI_LINE_MAX];
	std::vector<rgr> commands;
};

#endif
