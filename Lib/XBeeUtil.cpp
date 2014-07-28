/*
 * XBeeUtil.cpp
 */
#include <Arduino.h>
#include "XBeeUtil.h"
#include "Environment.h"
#include "nvram.h"

XBeeUtil XBee(Serial1);

// <maximus> Initialize XBee if NodeID is set to zero generate one
bool XBeeUtil::init()
{
	Env.reg("$name", [] (const char *var, char(*val)[ENVMAX]) {
		snprintf(*val, sizeof(nvram.xbee_name), "%s", nvram.xbee_name);
	}, [] (const char *var, const char *val) {
		strncpy(nvram.xbee_name, val, sizeof(nvram.xbee_name));
		store_nvram();
	});

	// First, can we talk
	port.begin(9600);
	if (!enterCommandMode()) {
		Serial.println("Could not enter command mode.");
		return false;
	}
	if (!talk("ATBD7\r", "Failed to set baud rate"))
		return false;

	if (!talk("ATAC\r", "Failed to apply baud rate"))
		return false;

	port.end();
	port.begin(115200);

	port.write("ATBD\r");
	const char *bd = getLine();
	if (bd == NULL || strncmp(bd, "7", XBEE_INPUTMAX) != 0) {
		Serial.printf("Error changing baud rate: %s\r\n", bd);
	}

	// Now set runtime parameters
	if (!talk("ATID aa995566\r", "Could not set PAN ID")) {
		return false;
	}

	snprintf(buffer, XBEE_INPUTMAX, "ATNI%s\r", nvram.xbee_name);
	if (!talk(buffer, "Could not set node ID")) {
		return false;
	}
	return true;
}

void XBeeUtil::status()
{
	Serial.println("--- XBee Status ---");
	port.write("ATHV\r");
	Serial.printf("Hardware version:     %s\r\n", getLine());
	port.write("ATVR\r");
	Serial.printf("Firmware version:     %s\r\n", getLine());
	port.write("ATAI\r");
	Serial.printf("Association status:   %s\r\n", getLine());
	port.write("ATNI\r");
	Serial.printf("Network ID:           %s\r\n", getLine());

	uint32_t hi, lo;
	port.write("ATSH\r");
	hi = strtoul(getLine(), NULL, 16);
	port.write("ATSL\r");
	lo = strtoul(getLine(), NULL, 16);

	Serial.printf("Serial Number:        %08x%08x\r\n", hi, lo);
	port.write("ATID\r");
	Serial.printf("PAN ID:               %s\r\n", getLine());
	Serial.println("-------------------");
}

bool XBeeUtil::isAssociated()
{
	if (!enterCommandMode())
		return false;
	port.write("ATAI\r");
	const char *r = getLine();
	return (r != NULL && strtoul(r, NULL, 16) == 0);
}

void XBeeUtil::discover()
{
	const char *r;
	nodes.clear();

	if (! isAssociated()) {
		Serial.println("XBee is not associated!");
		return;
	}

	port.write("ATNT\r");
	uint32_t timeout_millis = strtoul(getLine(), NULL, 16) * 100;
	port.write("ATND\r");

	while (1) {
		Node n;
		r = getLine(timeout_millis); // MY
		if (r == NULL || strncmp(r, "", XBEE_INPUTMAX) == 0) {
			break;
		}
		n.NetworkAddress = strtoul(r, NULL, 16);
		r = getLine(); // SH
		n.SerialHi = strtoul(r, NULL, 16);
		r = getLine(); // SL
		n.SerialLo = strtoul(r, NULL, 16);
		r = getLine(); // NI
		strncpy(n.NodeID, r, 20);
		r = getLine(); // PARENT_NETWORK_ADDR
		n.Parent = strtoul(r, NULL, 16);
		r = getLine(); // DEVICE_TYPE
		n.Type = strtoul(r, NULL, 16);
		r = getLine(); // STATUS
		n.Status = strtoul(r, NULL, 16);
		r = getLine(); // POFILE_ID
		n.Profile = strtoul(r, NULL, 16);
		r = getLine(); // MFGR
		n.Mfg = strtoul(r, NULL, 16);
		r = getLine(); // UNDOCUMENTED
		n.Extra = strtoul(r, NULL, 16);
		r = getLine(); // EMPTY LINE
		nodes.push_back(n);
	}

	Serial.println("--- Network Dump ---");
	for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		Serial.printf("ID: %d\r\n", (*it).NetworkAddress);
		Serial.printf("\tSerial: %08x%08x\r\n", (*it).SerialHi, (*it).SerialLo);
		if ((*it).Type == 0) {
			Serial.printf("\tType: Coordinator\r\n");
		}else if ((*it).Type == 1) {
			Serial.printf("\tType: Router\r\n");
		}else if ((*it).Type == 2) {
			Serial.printf("\tType: Endpoint\r\n");
		}else{
			Serial.printf("\tType: Unknown\r\n");
		}
		Serial.printf("\tName: %s\r\n", (*it).NodeID);
	}
}

bool XBeeUtil::talk(const char *command, const char *message, uint32_t time)
{
	port.write(command);
	if (!waitForOK()) {
		if (message != NULL)
			Serial.println(message);
		return false;
	}
	return true;
}

bool XBeeUtil::setDestinationNodeIdentifier(char *destinationNodeIdentifer)
{
	if (!enterCommandMode())
		return false;

	snprintf(buffer, XBEE_INPUTMAX, "ATDN%s\r", destinationNodeIdentifer);
	if (!talk(buffer, "Failed to set baud rate"))
		return false;

    return true;
}

bool XBeeUtil::setBroadcast()
{
	if (!enterCommandMode())
		return false;
	if (!talk("ATDH0", "Failed to set DH"))
		return false;
	if (!talk("ATDLFFFF", "Failed to set DL"))
		return false;
    return true;
}

bool XBeeUtil::enterCommandMode()
{
	// Track when we are in command mode. You can (and should) call
	// this function any time you want to run a command.
	if (last_access == 0 || (systick_millis_count - last_access) >= XBEE_COMMAND_TIMEOUT_MILLIS) {
		delay(XBEE_GUARD_TIME_MILLIS);
		port.write("+++");
		return waitForOK();
	}
	return true;
}

bool XBeeUtil::exitCommandMode()
{
	if (last_access != 0 && (systick_millis_count - last_access) < XBEE_COMMAND_TIMEOUT_MILLIS) {
	    port.write("ATCN\r");
	    last_access = 0;
	    return waitForOK();
	}
	return true;
}

bool XBeeUtil::waitForOK(uint32_t wait)
{
	const char *got = getLine(wait);
	return (got != NULL && strncmp(got, "OK", XBEE_INPUTMAX) == 0);
}

const char *XBeeUtil::getLine(uint32_t wait)
{
	uint32_t start = systick_millis_count;
	last_access = systick_millis_count;
	int pos = 0;
	while (pos < XBEE_INPUTMAX && (systick_millis_count - start) < wait) {
		if (port.available()) {
			buffer[pos] = port.read();
			if (buffer[pos] == 0x0d) {
				buffer[pos] = 0;
				return buffer;
			}
			pos++;
		}
	}
	return NULL;
}

void XBeeUtil::registerCommands(CLI &cc)
{
	cc.reg("ping", "ping -- Show all connected network nodes.", [] (std::vector<const char *> &v) {
		XBee.discover();
	});
}

void XBeeUtil::send(const char *s)
{
	exitCommandMode();
	port.write(s);
}
