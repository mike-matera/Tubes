/*
 * XBeeUtil.h
 *
 *  Created on: Jul 12, 2014
 *      Author: minty
 */

#ifndef XBEEUTIL_H_
#define XBEEUTIL_H_

#define XBEE_INPUTMAX               64
#define XBEE_COMMAND_TIMEOUT_MILLIS 10000
#define XBEE_GUARD_TIME_MILLIS      1000

#include <vector>

#include "HardwareSerial.h"
#include "cli.h"
#include "CommandListener.h"

/*
 * A struct that contains the information returned by the ATND command
 */
struct Node {
	uint16_t NetworkAddress;
	uint32_t SerialHi;
	uint32_t SerialLo;
	char NodeID[20];
	uint16_t Parent;
	uint8_t Type;
	uint8_t Status;
	uint16_t Profile;
	uint16_t Mfg;
	uint32_t Extra;
};

class XBeeUtil : public CommandListener {

public:
	XBeeUtil(HardwareSerial &p) : port(p) {/* can't talk to serial yet */}

	/*
	 * Intialize the XBee: Assumes the reset BAUD is 9600 will fail otherwise
	 */
	bool init();

	/*
	 * Retrieve and print the radio status.
	 */
	void status();

	/*
	 * Retrieve and print the network status
	 */
	void discover();

	/*
	 * Returns true if this node is connected to a network. Some functions will
	 * not operate unless it is (like discover())
	 */
	bool isAssociated();

	/*
	 * Set who we want to talk to.
	 */
	bool setDestinationNodeIdentifier(char *destinationNodeIdentifer);
	bool setBroadcast();

	/*
	 * Send the raw string to the XBee.
	 */
	void send(const char *);

	/*
	 * Initialization: Add XBee commands to the CLI
	 */
	void registerCommands(CLI &cc);
	virtual void onCommand(const std::vector<const char *> &c);
	virtual void onAssign(const char *var, const char *val);
	virtual void onReference(const char *var, char(*val)[ENVMAX]);

private:

	bool talk(const char *command, const char *message = NULL, uint32_t wait = XBEE_GUARD_TIME_MILLIS);
	bool enterCommandMode();
	bool exitCommandMode();
	bool waitForOK(uint32_t wait = XBEE_GUARD_TIME_MILLIS);
	const char *getLine(uint32_t wait = XBEE_GUARD_TIME_MILLIS);

private:

	HardwareSerial &port;
	uint32_t last_access = 0;
	char buffer[XBEE_INPUTMAX];
	std::vector<Node> nodes;
};

extern XBeeUtil XBee;

#endif /* XBEEUTIL_H_ */
