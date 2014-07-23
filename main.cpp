#include "WProgram.h"
#include "leds.h"
#include "Programs/Sparkle.h"
#include "Programs/TestProgram.h"
#include "Programs/Melt.h"
#include "Programs/FadeIn.h"
#include "Programs/FadeOut.h"
#include "Programs/Red.h"
#include "usb_serial.h"
#include "Lib/XBeeUtil.h"
#include <vector>
#include "cli.h"
#include "Environment.h"
#include "Programs.h"

// <minty> *IMPORTANT* this is used by programs to tell specific light elements to change
// these need to be in order and they need to be unique... if not unique, the last one with
// the conflicting value to turn on will turn red
#define XBEE_NODE_IDENTIFIER 0
#define XBEE_RESET_PIN       19

// <minty> xbee_serial1_control = true reads commands from the xbee instead
// of serial. If you're just plugging into USB, set this to false
// static bool xbee_serial1_control = false;

// <minty> xbee_connect controls whether or not commands sent over
// serial should be handled by the teensy or passed through to the xbee
static bool xbee_connect = false;

static CHSV DrawMem[nLEDs];

void xbee_early_init()
{
	// Reset the XBee
	digitalWrite(XBEE_RESET_PIN, LOW);
	pinMode(XBEE_RESET_PIN, OUTPUT);
	delay(100);
	pinMode(XBEE_RESET_PIN, INPUT);
	delay(100);

	Serial.println("Initializing XBee");
	if (! XBee.init()) {
		return;
	}
	XBee.status();
	Serial.println("Waiting for network association.");
	uint32_t start = systick_millis_count;
	while (systick_millis_count - start < 10000) {
		if (XBee.isAssociated()) {
			goto assoc;
		}
	}
	Serial.println("Did not associate in 10 seconds.");
	return;

	assoc:
	Serial.println("Scanning network");
	XBee.discover();
}

extern "C" int main(void)
{
    // <minty> DO NOT LOWER THIS
    // if you want xbee node identifier conflict detection to work.
    // The xbee's need a bit of time to load up and recognize the rest
    // of the network... when connected to usb, you don't need the delay
    // cause things are slowed down a bit or something. grumble grumble
	//    delay(5000);

	// Setup code...
	pinMode(13, OUTPUT);
	led_init();
	for (uint8_t i=0; i<nLEDs; i++) {
		led_set(i, 0);
	}
	led_show();
	analogReference(DEFAULT);
	randomSeed(analogRead(8));

	// <maximus> USB serial is fucked. The integer argument to the
	// constructor is ignored. See usb_serial.h, line 69
	// The host sets the baud and we can only read it. It seems that
	// begin() is this way for compatibility with Arduino.
    Serial.begin(115200);
	// XXX: FIXME: Handle BAUD: Serial1.begin(9600);

	// <minty> status check
    for (int i=0; i<nLEDs; i++)
        led_set(i, CHSV(96, 255, 50));

    xbee_early_init();

    Progs.registerProgram("melt", new Melt());
    Progs.registerProgram("sparkle", new Sparkle());
    Progs.registerProgram("red", new Red());
    Progs.registerProgram("fadein", new FadeIn());
    Progs.registerProgram("fadeout", new FadeOut());
    Progs.registerProgram("test", new TestProgram());

	CLI cc(Serial);
	cc.help();

	uint32_t linkbaud = Serial.baud();
	while (1) {
		if (xbee_connect) {
			if (linkbaud != Serial.baud()) {
				linkbaud = Serial.baud();
				Serial1.flush();
				Serial1.end();
				Serial1.begin(linkbaud);
			}
			if (Serial.available()) {
				Serial1.write(Serial.read());
			}
			if (Serial1.available()) {
				Serial.write(Serial1.read());
			}
			continue;
		}

		// Make sure the menu gets love
		cc.do_cli();
		Progs.render(DrawMem);

		// Render the HSV buffer onto our RGB pixels
		for (int i=0; i<nLEDs; i++) {
			led_set(i, DrawMem[i]);
		}
		led_show();
	}
}
