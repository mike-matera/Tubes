/*
 * XBeeUtil.h
 *
 *  Created on: Jul 12, 2014
 *      Author: minty
 */

#ifndef XBEEUTIL_H_
#define XBEEUTIL_H_

#define XBEE_INPUTMAX 64

class XBeeUtil {
public:
    static void clear();
    static bool setNodeIdentifier(char *nodeIdentifier);
    static void getNodeIdentifier(char *nodeIdentifier);
    static bool setDestinationNodeIdentifier(char *destinationNodeIdentifer);
    static bool setBroadcast();
    static bool enterCommandMode();
    static bool exitCommandMode();
    static bool waitForOK();
};

#endif /* XBEEUTIL_H_ */
