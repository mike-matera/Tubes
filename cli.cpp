
#include <cstring>
#include <vector>

#include "Arduino.h"

#include "cli.h"

void CLI::do_cli() {
	if (dev.available()) {
		input[pos] = dev.read();
		if (input[pos] == '\r') {
			input[pos] = 0;
			if (echo)
				Serial.println();
			exec();
			pos = 0;
			prompt();
		}else{
			if (echo)
				dev.write(input[pos]);
			pos++;
		}
	}
}

CLI::token CLI::tok(const char *txt)
{
	token t;
	t.d = txt;
	if (ss == SCANNING) {
		if (t.d[0] == '$') {
			t.t = IDENT;
			goto ident_done;
		}
		if (strcmp(t.d, "help") == 0 || strcmp(t.d, "echo") == 0 || strcmp(t.d, "set") == 0) {
			t.t = COMMAND;
			goto ident_done;
		}
		for (std::vector<rgr>::iterator it = commands.begin(); it != commands.end(); it++) {
			if (strcmp(t.d, (*it).name) == 0) {
				t.t = COMMAND;
				goto ident_done;
			}
		}
		t.t = VALUE;
		ss = CONSUME;
	}else if (ss == CONSUME) {
		t.t = VALUE;
	}
	ident_done:
	return t;
}

void CLI::help() {
	dev.println("");
	dev.println("Simple command syntax:");
	dev.println("  <command> [<args>]");
	dev.println("");
	dev.println("Commands:");
	dev.println("  set $var <value>        -- Assign the program variable");
	dev.println("  echo $var               -- Display the program variable");
	for (std::vector<rgr>::iterator it = commands.begin(); it != commands.end(); it++) {
		dev.printf("  %s\r\n", (*it).help);
	}
	dev.println("  help                    -- Display this message");
	dev.println("");
}

void CLI::prompt() {
	if (echo) {
		Serial.print("cmd> ");
	}
}

/*
 * Not as good as an ANTLR parser, but much smaller.
 */
void CLI::exec() {
	ss = SCANNING;
	std::vector<token> tokens;
	const char *t;
	t = strtok(input, DELIMETERS);
	do {
		tokens.push_back(tok(t));
	}while ((t = strtok(NULL, DELIMETERS)) != NULL);

	if (tokens[0].t != COMMAND) {
		if (strcmp("", tokens[0].d) != 0) {
			Serial.println("Invalid command.");
		}
		return;
	}
	if (strcmp("set", tokens[0].d) == 0 && tokens[1].t == IDENT) {
		for (std::vector<env>::iterator it = vars.begin(); it != vars.end(); it++) {
			if (strcmp((*it).var, tokens[1].d) == 0) {
				(*it).cmd->onAssign(tokens[1].d, tokens[2].d);
			}
		}
	}else if (strcmp("echo", tokens[0].d) == 0 && tokens[1].t == IDENT) {
		for (std::vector<env>::iterator it = vars.begin(); it != vars.end(); it++) {
			if (strcmp((*it).var, tokens[1].d) == 0) {
				(*it).cmd->onReference(tokens[1].d, &buffer);
				Serial.println(buffer);
				goto done;
			}
		}
	}else if (strcmp("help", tokens[0].d) == 0) {
		help();
	}else{
		// Search command list
		for (std::vector<rgr>::iterator it = commands.begin(); it != commands.end(); it++) {
			if (strcmp((*it).name, tokens[0].d) == 0) {
				// Found
				std::vector<const char *> v;
				for (unsigned int i=0; i<tokens.size(); i++) {
					v.push_back(tokens[i].d);
				}
				(*it).cmd->onCommand(v);
				goto done;
			}
		}
		Serial.println("Invalid command.");
	}

	done:

	return;
}

void CLI::registerCommand(const char *cmd, const char *help, CommandListener *cb)
{
	rgr n;
	unsigned int l = strnlen(cmd, CLI_LINE_MAX-1);
	n.name = new char[l+1];
	strncpy(n.name, cmd, l+1);

	l = strnlen(help, CLI_LINE_MAX-1);
	n.help = new char[l+1];
	strncpy(n.help, help, l+1);

	n.cmd = cb;

	commands.push_back(n);
}

void CLI::registerVariable(const char *var, CommandListener *cb)
{
	env n;
	unsigned int l = strnlen(var, CLI_LINE_MAX-1);
	n.var = new char[l+1];
	strncpy(n.var, var, l+1);

	n.cmd = cb;

	vars.push_back(n);
}

void CLI::exec(const char *command)
{
	strncpy(input, command, CLI_LINE_MAX);
	exec();
}
