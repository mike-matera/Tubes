
#include <cstring>
#include <vector>

#include "Arduino.h"

#include "cli.h"
#include "Environment.h"
#include "Programs.h"

const static int numcmds = 4;
const static char *commands[] = {"program", "set", "echo", "help"};

void CLI::do_cli() {
	if (dev.available()) {
		input[pos] = dev.read();
		if (echo)
			dev.write(input[pos]);
		if (input[pos] == '\r') {
			input[pos] = 0;
			exec();
			pos = 0;
		}else{
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
		for (int i=0; i<numcmds; i++) {
			if (strcmp(t.d, commands[i]) == 0) {
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
	dev.println("Simple command syntax:");
	dev.println("  <command> [<args>]");
	dev.println("");
	dev.println("Commands:");
	dev.println("  program <progname> ...  -- Set the program stack");
	dev.println("  set $var <value>        -- Assign the program variable");
	dev.println("  echo $var               -- Display the program variable");
	dev.println("");
}

void CLI::exec() {
	ss = SCANNING;
	std::vector<token> tokens;
	const char *t;
	t = strtok(input, DELIMETERS);
	do {
		tokens.push_back(tok(t));
	}while ((t = strtok(NULL, DELIMETERS)) != NULL);

	if (tokens[0].t != COMMAND) {
		help();
		return;
	}
	if (strcmp("program", tokens[0].d) == 0) {
		Progs.clear();
		for (unsigned int i=1; i<tokens.size(); i++) {
			Progs.pushProgram(tokens[i].d);
		}
	}else if (strcmp("set", tokens[0].d) == 0 && tokens[1].t == IDENT) {
		for (unsigned int i=2; i<tokens.size(); i++) {
			Env.set(tokens[1].d, tokens[i].d);
		}
	}else if (strcmp("echo", tokens[0].d) == 0 && tokens[1].t == IDENT) {
		Serial.printf("%s\r\n", Env.get(tokens[1].d));
	}else{
		help();
	}
	/*
	Serial.println("got:");
	for (std::vector<token>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		Serial.printf("d: %s t: %d\r\n", (*it).d, (*it).t);
	}
	*/
}
