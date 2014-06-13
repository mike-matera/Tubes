#include "WProgram.h"
#include "leds.h"
#include "Programs/Sparkle.h"
#include "Programs/TestProgram.h"
#include "usb_serial.h"

static IntervalTimer timer;
volatile static int do_next_frame = 0;

#define INPUTMAX 256
static char input[INPUTMAX];
static int pos = 0;

static Program *p = NULL;

void timeExpired() {
	do_next_frame++;
	if (do_next_frame > 1) {
		digitalWriteFast(13, HIGH);
	}else{
		digitalWriteFast(13, LOW);
	}
}

void setProgram(Program *n, float fps) {
	if (p != NULL) {
		timer.end();
		delete p;
	}
	p = n;
	timer.begin(timeExpired, (1 / fps) * 1000000);
}

void emit() {
	if (strncmp("test", input, INPUTMAX) == 0) {
		TestProgram *p = new TestProgram();
		setProgram(p, 10);
	}else if (strncmp("sparkle", input, INPUTMAX) == 0) {
		Sparkle *s = new Sparkle();
		setProgram(s, 30);
	}
	Serial.printf("Your options are:\r\n");
	Serial.printf("\ttest - Run a test pattern\r\n");
	Serial.printf("\tsparkle - Run a Mike's sparkle pattern\r\n");
	Serial.printf("\r\n>");
}

void menu() {
	while (Serial.available()) {
		input[pos] = Serial.read();
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

extern "C" int main(void)
{
	// Setup code...
	pinMode(13, OUTPUT);
	led_init();
	for (uint8_t i=0; i<nLEDs; i++) {
		led_set(i, 0);
	}
	led_show();
	analogReference(DEFAULT);
	randomSeed(analogRead(12));

	Serial.begin(115200);

	do_next_frame = 0;

	while (1) {

		// Do this to park:
		//asm volatile ("WFI");

		// Do this to dither:
		led_show();

		// Be sure to run the menu
		menu();

		if (do_next_frame && p != NULL) {
			do_next_frame = 0;
			p->render();
			led_show();
		}
	}
}
