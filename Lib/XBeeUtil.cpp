/*
 * XBeeUtil.cpp
 */

#include <Arduino.h>
#include "XBeeUtil.h"

XBeeUtil XBee(Serial1);

// <maximus> Initialize XBee if NodeID is set to zero generate one
bool XBeeUtil::init(int nodeid)
{
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

	if (!talk("ATID aa995566\r", "Could not set PAN ID")) {
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
	port.write("ATMY\r");
	Serial.printf("Network address:      %s\r\n", getLine());

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
	port.write("ATAI\r");
	const char *r = getLine();
	return (r != NULL && strtoul(r, NULL, 16) == 0);
}

void XBeeUtil::discover()
{
	const char *r;
	nodes.clear();

	if (! isAssociated())
		return;

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

// <minty> write some carriage returns to the xbee and clear what's on it's input
void XBeeUtil::clear()
{
    port.write("\r\r\r");
    while (port.available())
        port.read();
}

bool XBeeUtil::setNodeIdentifier(char *nodeIdentifier)
{
    uint8_t nodeLineIndex = 0;
    char input[XBEE_INPUTMAX];
    char my[XBEE_INPUTMAX];
    byte pos = 0;
    bool conflict = false;
//    Serial.printf("attempting to set node identifier to %s\r\n", nodeIdentifier);

//    std::vector<uint32_t> others;

    if (enterCommandMode())
    {
        // <minty> first we gotta check for conflicts...
        // <minty> get MY 16-bit network address
        port.write("ATMY");
        port.write("\r");
        const char *got = getLine();
        if (got != NULL)
        	strncpy(my, got, XBEE_INPUTMAX);

	    Serial.printf("got MY: %s [%d]\r\n", my, pos);

        port.write("ATND");
        port.write(nodeIdentifier);
        port.write("\r");
        delay(200);

        // <minty> loop over all the results...
        // each node separated by two carriage returns
        // we're looking for the 4th line...
        pos = 0;
        elapsedMillis timer = 0;
        while ((timer < 1500) && !conflict)
	        while (port.available()) {
		        input[pos] = port.read();
//		        Serial.print(input[pos], HEX);
		        if (input[pos] == '\r' || pos == XBEE_INPUTMAX-1) {
			        input[pos] = 0;
//			        Serial.printf("[%d,%d] %s\r\n", nodeLineIndex, pos, input);

			        if ((pos == 0) && (nodeLineIndex == 10)) {
//			            Serial.println("END OF NODE");
				        nodeLineIndex = 0;
				        pos = 0;
				        timer = 0;
			        }
			        // <minty> probably don't need to go to the trouble of confirming the identifier
			        // matches what we asked for, but what the hell
			        else
			        {
			            if (nodeLineIndex == 0)
			            {
			                // <minty> stupid fucking ATMY drops leading zeros while ATND does not
			                char *address = input;
			                if (input[0] == '0')
			                    address++;
			                if (strcmp(my, address))
                            {
                                conflict = true;
//                                Serial.println("conflict!!!!!");
                            }
			            }

			            nodeLineIndex++;
			            pos = 0;
			        }
		        } else {
			        pos++;
		        }
			    delay(10);
	        }

	    if (conflict)
        {
//			Serial.println("found conflicting node!");
            exitCommandMode();
            return false;
        }
//	    else
//			Serial.printf("setting node identifier to %s\r\n", nodeIdentifier);

        // <minty> no conflicts, set our node identifier
        port.write("ATNI");
        port.write(nodeIdentifier);
        port.write("\r");
        if (!waitForOK())
        {
            exitCommandMode();
            return false;
        }
        return exitCommandMode();
    }

    return false;
}

void XBeeUtil::getNodeIdentifier(char *nodeIdentifier)
{
    if (enterCommandMode())
    {
        port.write("ATNI\r");
        const char *got = getLine();
        if (got != NULL) {
        	strncpy(nodeIdentifier, got, XBEE_INPUTMAX);
        }
	    exitCommandMode();
	}
}

bool XBeeUtil::setDestinationNodeIdentifier(char *destinationNodeIdentifer)
{
    if (enterCommandMode())
    {
        port.write("ATDN");
        port.write(destinationNodeIdentifer);
        port.write("\r");
        return waitForOK();
    }
    return false;
}

bool XBeeUtil::setBroadcast()
{
    bool rv = false;

    if (enterCommandMode())
    {
        port.write("ATDH0\r");
        if (waitForOK())
        {
            port.write("ATDLFFFF\r");
            if (waitForOK())
                rv = true;
        }
        exitCommandMode();
    }

    return rv;
}

bool XBeeUtil::enterCommandMode()
{
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
	//Serial.printf("DEBUG: waitForOK(): \"%s\"\r\n", got);
	return (got != NULL && strncmp(got, "OK", XBEE_INPUTMAX) == 0);
}

const char *XBeeUtil::getLine(uint32_t wait)
{
	uint32_t start = systick_millis_count;
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
