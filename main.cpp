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

// <minty> *IMPORTANT* this is used by programs to tell specific light elements to change
// these need to be in order and they need to be unique... if not unique, the last one with
// the conflicting value to turn on will turn red
#define XBEE_NODE_IDENTIFIER 0

#define INPUTMAX 256
static char input[INPUTMAX];
static int pos = 0;

// <minty> xbee_serial1_control = true reads commands from the xbee instead
// of serial. If you're just plugging into USB, set this to false
static bool xbee_serial1_control = true;

// <minty> xbee_connect controls whether or not commands sent over
// serial should be handled by the teensy or passed through to the xbee
static bool xbee_connect = false;

typedef struct {
	Renderable *r;
	uint32_t t;
} res;

static std::vector<res> Programs;
static CHSV DrawMem[nLEDs];

void clear_programs() {
	for (std::vector<res>::iterator it = Programs.begin(); it != Programs.end(); it++) {
		(*it).r->teardown();
		delete (*it).r;
	}
	Programs.clear();
}

void emit() {
	res n;

	if ((input[0] == 'p') and (input[1] = ':'))
	    if (strncmp("test", input + 2, INPUTMAX) == 0) {
		    clear_programs();
		    n = {new TestProgram(), 0};
		    n.r->setup();
		    Programs.push_back(n);
            n = {new FadeIn(), 0};
            n.r->setup();
            Programs.push_back(n);
	    }else if (strncmp("sparkle", input + 2, INPUTMAX) == 0) {
		    clear_programs();
		    n = {new Sparkle(), 0};
		    n.r->setup();
		    Programs.push_back(n);
            n = {new FadeIn(), 0};
            n.r->setup();
            Programs.push_back(n);
	    }else if (strncmp("xbee", input + 2, INPUTMAX) == 0) {
		    clear_programs();
		    xbee_connect = true;
	    }else if (strncmp("melt", input + 2, INPUTMAX) == 0) {
		    clear_programs();
		    n = {new Melt(), 0};
		    n.r->setup();
		    Programs.push_back(n);
            n = {new FadeIn(), 0};
            n.r->setup();
            Programs.push_back(n);
	    }else if (strncmp("sparklemelt", input + 2, INPUTMAX) == 0) {
		    clear_programs();
            n = {new Melt(), 0};
            n.r->setup();
            Programs.push_back(n);
            n = {new Sparkle(), 0};
            n.r->setup();
            Programs.push_back(n);
            n = {new FadeIn(), 0};
            n.r->setup();
            Programs.push_back(n);
	    }else if (strncmp("fadeOut", input + 2, INPUTMAX) == 0) {
		    //clear_programs();
		    n = {new FadeOut(), 0};
		    n.r->setup();
		    Programs.push_back(n);
	    }else if (strncmp("red", input + 2, INPUTMAX) == 0) {
		    clear_programs();
		    n = {new Red(), 0};
		    n.r->setup();
		    Programs.push_back(n);
            n = {new FadeIn(), 0};
            n.r->setup();
            Programs.push_back(n);
	    }

	if (!xbee_serial1_control)
	{
        Serial.printf("Your options are:\r\n");
        Serial.printf("\ttest - Run a test pattern\r\n");
        Serial.printf("\tsparkle - Run Mike's sparkle pattern\r\n");
        Serial.printf("\tmelt - Run casey's melt pattern\r\n");
        Serial.printf("\tsparklemelt - yep\r\n");
        Serial.printf("\txbee - Connect serial to XBee\r\n");
        Serial.printf("\r\n>");
	}
}

void menu() {
    Stream *ear;

    if (xbee_serial1_control)
        ear = &Serial1;
    else
        ear = &Serial;

	while (ear->available()) {
		input[pos] = ear->read();
        if (!xbee_serial1_control)
            Serial.write(input[pos]);
		if (input[pos] == '\r' || pos == INPUTMAX-1) {
			input[pos] = 0;
			if (pos > 0 && input[pos-1] == '\r') {
				input[pos-1] = 0;
			}
			emit();
			pos = 0;
		}else{
			pos++;
		}
	}
}

s
extern "C" int main(void)
{
    // <minty> DO NOT LOWER THIS
    // if you want xbee node identifier conflict detection to work.
    // The xbee's need a bit of time to load up and recognize the rest
    // of the network... when connected to usb, you don't need the delay
    // cause things are slowed down a bit or something. grumble grumble
    delay(5000);
	// Setup code...
	pinMode(13, OUTPUT);
	led_init();
	for (uint8_t i=0; i<nLEDs; i++) {
		led_set(i, 0);
	}
	led_show();
	analogReference(DEFAULT);
	randomSeed(analogRead(8));

	Serial.begin(115200);
	Serial1.begin(9600);

	// <minty> status check
    for (int i=0; i<nLEDs; i++)
        led_set(i, CHSV(96, 255, 50));

	// <minty> xbee setup
	bool noError = true;
    if (xbee_serial1_control)
    {
        XBeeUtil::clear();
        sprintf(input, "%d", XBEE_NODE_IDENTIFIER);
        noError = XBeeUtil::setNodeIdentifier(input);
    }

	res n;
	clear_programs();
	if (noError)
	{
        n = {new Melt(), 0};
        n.r->setup();
        Programs.push_back(n);

        n = {new Sparkle(), 0};
        n.r->setup();
        Programs.push_back(n);

        n = {new FadeIn(), 0};
        n.r->setup();
        Programs.push_back(n);
	}
	else
	{
        n = {new Red(), 0};
        n.r->setup();
        Programs.push_back(n);
	}

	emit();
	while (1) {
		if (xbee_connect) {
			if (Serial.available()) {
				Serial1.write((unsigned char *) input, Serial.readBytes(input, INPUTMAX));
			}
			if (Serial1.available()) {
				Serial.write((unsigned char *) input, Serial1.readBytes(input, INPUTMAX));
			}
			continue;
		}

		// Make sure the menu gets love
		menu();

		// Run the programs
		for (std::vector<res>::iterator it = Programs.begin(); it != Programs.end(); it++) {
			res &r = *it;
			if (r.t <= systick_millis_count)
				r.t = r.r->render(DrawMem) + systick_millis_count;
		}

		// Render the HSV buffer onto our RGB pixels
		for (int i=0; i<nLEDs; i++) {
			led_set(i, DrawMem[i]);
		}
		led_show();
	}
}
