#include "loader.h"

extern "C" {

void __attribute__((section (".bootloader"), long_call)) reloader() {
	//Serial.println("RAM ME!");
}

}
