#include "WProgram.h"
#include "leds.h"
#include "Programs/Sparkle.h"
#include "Programs/TestProgram.h"
#include "Programs/Melt.h"
#include "usb_serial.h"
#include <vector>

#define INPUTMAX 256
static char input[INPUTMAX];
static int pos = 0;

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
	if (strncmp("test", input, INPUTMAX) == 0) {
		clear_programs();
		n = {new TestProgram(), 0};
		n.r->setup();
		Programs.push_back(n);
	}else if (strncmp("sparkle", input, INPUTMAX) == 0) {
		clear_programs();
		n = {new Sparkle(), 0};
		n.r->setup();
		Programs.push_back(n);
	}else if (strncmp("melt", input, INPUTMAX) == 0) {
		clear_programs();
		n = {new Melt(), 0};
		n.r->setup();
		Programs.push_back(n);
	}
	Serial.printf("Your options are:\r\n");
	Serial.printf("\ttest - Run a test pattern\r\n");
	Serial.printf("\tsparkle - Run Mike's sparkle pattern\r\n");
	Serial.printf("\tmelt - Run casey's melt pattern\r\n");
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

	while (1) {
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
