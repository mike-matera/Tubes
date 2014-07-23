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

class XBeeUtil {

public:
	XBeeUtil(HardwareSerial &p) : port(p) {/* can't talk to serial yet */}
	bool init(int node_id=0);
	void status();
	void discover();
	bool isAssociated();

	bool setNodeIdentifier(char *nodeIdentifier);
	void getNodeIdentifier(char *nodeIdentifier);
	bool setDestinationNodeIdentifier(char *destinationNodeIdentifer);
	bool setBroadcast();

private:
	void clear();
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
