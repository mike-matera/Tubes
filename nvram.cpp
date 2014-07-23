#include "nvram.h"
#include "Arduino.h"

eeprom_t nvram;

void init_nvram()
{
	eeprom_read_block(&nvram, 0, sizeof(eeprom_t));
}

void store_nvram()
{
	eeprom_write_block(&nvram, 0, sizeof(eeprom_t));
}
