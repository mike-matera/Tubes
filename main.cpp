#include <vector>
#include <sstream>

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
#include "cli.h"
#include "Programs.h"
#include "nvram.h"

#define XBEE_RESET_PIN       19
#define LED_PIN              13

// <maximus> Add this if you want to avoid initializing the XBee.
// XBee initialization takes time and if you're testing programs
// it's annoying. Also good for unpatched Rev A boards
//
//#define NO_XBEE_INIT

// Two command line interpreters. One for the USB serial port
// and one for XBee. They are able to operate independently.
static CLI cc(Serial);
static CLI cx(Serial1);

bool xbee_early_init()
{
#ifndef NO_XBEE_INIT
	// Reset the XBee
	digitalWrite(XBEE_RESET_PIN, LOW);
	pinMode(XBEE_RESET_PIN, OUTPUT);
	delay(100);
	pinMode(XBEE_RESET_PIN, INPUT);
	delay(100);

	Serial.println("Initializing XBee");
	if (!XBee.init()) {
	    Serial.println("init failed");
		return false;
	}
	XBee.status();
	Serial.println("Waiting for network association.");
	uint32_t start = systick_millis_count;
	while (systick_millis_count - start < 20000) {
		if (XBee.isAssociated()) {
			goto assoc;
		}
	}
	Serial.println("Did not associate in 20 seconds.");
	return false;

	assoc:
	Serial.println("Scanning network");
	XBee.discover();
	XBee.setBroadcast();
#endif

	return true;
}

void nvram_early_init() {
	init_nvram();
	Serial.println("NVRAM Loaded");
	Serial.printf("  XBee Name  : %s\r\n", nvram.xbee_name);
}

extern "C" int main(void)
{
	static bool xbee_connect = false;
	static bool reboot = false;

	analogReference(DEFAULT);
	pinMode(LED_PIN, OUTPUT);
	led_init();

	begin:

	for (uint8_t i=0; i<nLEDs; i++) {
		led_set(i, 0);
	}
	led_show();

	// Get a random seed
	randomSeed(analogRead(8));

	// <maximus> USB serial is fucked. The integer argument to the
	// constructor is ignored. See usb_serial.h, line 69
	// The host sets the baud and we can only read it. It seems that
	// begin() is this way for compatibility with Arduino.
    Serial.begin(115200);

	// <minty> status check
    for (int i=0; i<nLEDs; i++)
        led_set(i, CHSV(96, 255, 50));

    // <maximus> Uncomment this if you want to be able to see the
    // nvram printout. My terminal emulator doesn't reconnect fast
    // enough to see it at boot unless there's some delay.
    delay(5000);

    // Load the NVRAM into our struct
    nvram_early_init();

    // Register the programs that we can run. The names will be the
    // ones used by the CLI
    Melt *prog_melt = new Melt();
    Sparkle *prog_sparkle = new Sparkle();
    Red *prog_red = new Red();
    FadeIn *prog_fadein = new FadeIn();
    FadeOut *prog_fadeout = new FadeOut();
    TestProgram *prog_testprogram = new TestProgram();

    Progs.registerProgram("melt", prog_melt);
    Progs.registerProgram("sparkle", prog_sparkle);
    Progs.registerProgram("red", prog_red);
    Progs.registerProgram("fadein", prog_fadein);
    Progs.registerProgram("fadeout", prog_fadeout);
    Progs.registerProgram("test", prog_testprogram);

    // Reset and configure XBee
    bool xbee_initialized = xbee_early_init();

    // Register the program control commands on both CLIs
    Progs.registerCommands(cc);
    Progs.registerCommands(cx);

    // Register XBee commands on the USB serial CLI only
    XBee.registerCommands(cc);

    if (!xbee_initialized)
        Progs.pushProgram("red");

    class XbeeConnect : public CommandListener {
    	virtual void onCommand(const std::vector<const char *> &args) {
    		xbee_connect = true;
    	}
    } xc_cmd;
    cc.registerCommand("xbee", "xbee -- Connect XBee", &xc_cmd);

    class XbeeCommand : public CommandListener {
    	virtual void onCommand(const std::vector<const char *> &args) {
        	char cmd[CLI_LINE_MAX];
        	unsigned int cnt = 0;
        	for (unsigned int i=1; i<args.size(); i++) {
            	cnt += snprintf(&cmd[cnt], CLI_LINE_MAX - cnt, "%s ", args[i]);
        	}
        	snprintf(&cmd[cnt], CLI_LINE_MAX - cnt, "\r\n");
        	XBee.send(cmd);
        	cx.exec(cmd);
    	}
    } xc_snd;
    cc.registerCommand("xsend", "xsend -- Send command over XBee", &xc_snd);

    class Reboot : public CommandListener {
    	virtual void onCommand(const std::vector<const char *> &args) {
    		reboot = true;
    	}
    } rb_cmd;
    cc.registerCommand("reboot", "reboot -- Restart the program", &rb_cmd);

    // Do not echo a prompt or help to the xbee command line
    cx.setEcho(false);
    cc.help();
	cc.prompt();

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

		if (reboot)
			goto reboot;

		cc.do_cli();
		cx.do_cli();

		Progs.render();
	}

	reboot:

	// Attempt teardown
	reboot = false;
	xbee_connect = false;

	cc.reset();
	cx.reset();

	Progs.reset();

    delete prog_melt;
    delete prog_sparkle;
    delete prog_red;
    delete prog_fadein;
    delete prog_fadeout;
    delete prog_testprogram;

    for (int i=0; i<nLEDs; i++) {
    	HSVPixels[i] = CHSV(0,0,0);
    	RGBPixels[i] = 0;
    	led_set(i,0);
    }

    goto begin;
}
