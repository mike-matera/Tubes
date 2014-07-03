#include "WProgram.h"
#include "leds.h"
#include "Programs/Sparkle.h"
#include "Programs/TestProgram.h"
#include "usb_serial.h"
#include "XBee.h"
#include <vector>

#define INPUTMAX 256
static char input[INPUTMAX];
static int pos = 0;
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
	}else if (strncmp("xbee", input, INPUTMAX) == 0) {
		clear_programs();
		xbee_connect = true;
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

/*
uint8_t ssRX = 8;
uint8_t ssTX = 9;
XBee xbee = XBee();
// serial high
uint8_t shCmd[] = {'S','H'};
// serial low
uint8_t slCmd[] = {'S','L'};
// association status
uint8_t assocCmd[] = {'A','I'};
AtCommandRequest atRequest = AtCommandRequest(shCmd);
AtCommandResponse atResponse = AtCommandResponse();

#define XBEE_RESET FUCKED

void sendAtCommand() {
  Serial.println("Sending command to the XBee");

  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {
        Serial.print("Command [");
        Serial.print(atResponse.getCommand()[0]);
        Serial.print(atResponse.getCommand()[1]);
        Serial.println("] was successful!");

        if (atResponse.getValueLength() > 0) {
          Serial.print("Command value length is ");
          Serial.println(atResponse.getValueLength(), DEC);

          Serial.print("Command value: ");

          for (int i = 0; i < atResponse.getValueLength(); i++) {
            Serial.print(atResponse.getValue()[i], HEX);
            Serial.print(" ");
          }

          Serial.println("");
        }
      }
      else {
        Serial.print("Command return error code: ");
        Serial.println(atResponse.getStatus(), HEX);
      }
    } else {
      Serial.print("Expected AT response but got ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
    }
  } else {
    // at command failed
    if (xbee.getResponse().isError()) {
      Serial.print("Error reading packet.  Error code: ");
      Serial.println(xbee.getResponse().getErrorCode());
    }
    else {
      Serial.println("No response from radio");
    }
  }
}
*/

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
	Serial1.begin(9600);

	/*
	delay(1000);
	Serial.println("Waiting for XBEE");
	delay(5000);

	 // get SH
	sendAtCommand();

	// set command to SL
	atRequest.setCommand(slCmd);
	sendAtCommand();

	// set command to AI
	atRequest.setCommand(assocCmd);
	sendAtCommand();
	*/
	/*
	delay(5000);
	Serial.println("You are talking to XBEE");
	while (1) {
		if (Serial.available()) {
			char c = Serial.read();
			Serial1.write(c);
			Serial.write(c);
		}
		if (Serial1.available()) {
			Serial.write(Serial1.read());
		}
	}
	*/
	res n;
	clear_programs();
	n = {new TestProgram(), 0};
	n.r->setup();
	Programs.push_back(n);

	while (1) {
		if (xbee_connect) {
			if (Serial.available()) {
				Serial1.write((uint8_t) Serial.read());
				Serial1.flush();
			}
			if (Serial1.available()) {
				Serial.write((uint8_t) Serial1.read());
				Serial.flush();
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
