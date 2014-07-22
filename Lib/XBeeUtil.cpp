/*
 * XBeeUtil.cpp
 */

#include "XBeeUtil.h"
#include <Arduino.h>

// <minty> write some carriage returns to the xbee and clear what's on it's input
void XBeeUtil::clear()
{
    Serial1.write("\r\r\r");
    while (Serial1.available())
        Serial1.read();
}

bool XBeeUtil::setNodeIdentifier(char *nodeIdentifier)
{
    uint8_t nodeLineIndex = 0;
    char input[XBEE_INPUTMAX];
    char my[XBEE_INPUTMAX];
    char tmpChar;
    byte pos = 0;
    bool conflict = false;
//    Serial.printf("attempting to set node identifier to %s\r\n", nodeIdentifier);

    if (enterCommandMode())
    {
        // <minty> first we gotta check for conflicts...
        // <minty> get MY 16-bit network address
        Serial1.write("ATMY");
        Serial1.write("\r");
        delay(300);
	    while (Serial1.available())
	    {
		    my[pos] = Serial1.read();
		    if (my[pos] == '\r' || pos == XBEE_INPUTMAX-1)
			    my[pos] = 0;
		    else
			    pos++;
	    }
//	    Serial.printf("got MY: %s [%d]\r\n", my, pos);

        Serial1.write("ATND");
        Serial1.write(nodeIdentifier);
        Serial1.write("\r");
        delay(200);

        // <minty> loop over all the results...
        // each node separated by two carriage returns
        // we're looking for the 4th line...
        pos = 0;
        elapsedMillis timer = 0;
        while ((timer < 1500) && !conflict)
	        while (Serial1.available()) {
		        input[pos] = Serial1.read();
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
        Serial1.write("ATNI");
        Serial1.write(nodeIdentifier);
        Serial1.write("\r");
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
    char input[XBEE_INPUTMAX];
    byte pos = 0;

    if (enterCommandMode())
    {
        Serial1.write("ATNI\r");

	    while (Serial1.available()) {
		    input[pos] = Serial1.read();
		    if (input[pos] == '\r' || pos == XBEE_INPUTMAX-1) {
			    input[pos] = 0;
			    if (pos > 0 && input[pos-1] == '\r') {
				    input[pos-1] = 0;
			    }
			    strncpy(nodeIdentifier, input, pos);
			    pos = 0;
		    }else{
			    pos++;
		    }
	    }
	    exitCommandMode();
	}
}

bool XBeeUtil::setDestinationNodeIdentifier(char *destinationNodeIdentifer)
{
    if (enterCommandMode())
    {
        Serial1.write("ATDN");
        Serial1.write(destinationNodeIdentifer);
        Serial1.write("\r");
        return waitForOK();
    }
    return false;
}

bool XBeeUtil::setBroadcast()
{
    bool rv = false;

    if (enterCommandMode())
    {
        Serial1.write("ATDH0\r");
        if (waitForOK())
        {
            Serial1.write("ATDLFFFF\r");
            if (waitForOK())
                rv = true;
        }
        exitCommandMode();
    }

    return rv;
}

bool XBeeUtil::enterCommandMode()
{
    delay(1000);
    Serial1.write("+++");
    // <minty> command mode takes longer to be recognized and return an OK
    delay(800);

    return waitForOK();
}

bool XBeeUtil::exitCommandMode()
{
    Serial1.write("ATCN\r");

    return waitForOK();
}

bool XBeeUtil::waitForOK()
{
    elapsedMillis timer = 0;
    char input[XBEE_INPUTMAX];
    byte pos = 0;

    // <minty> the commands take a hair to populate the serial port
    delay(200);
	while (Serial1.available()) {
		input[pos] = Serial1.read();
		if (input[pos] == '\r' || pos == XBEE_INPUTMAX-1) {
			input[pos] = 0;
			if (pos > 0 && input[pos-1] == '\r') {
				input[pos-1] = 0;
			}

            if (!strncmp(input, "OK", 2))
                return true;
            else
                return false;

			pos = 0;
		} else {
			pos++;
		}
	}

    return false;
}
