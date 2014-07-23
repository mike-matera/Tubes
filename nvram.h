/*
 * nvram.h
 *
 *  Created on: Jul 23, 2014
 *      Author: maximus
 */
#ifndef _NVRAM_H
#define _NVRAM_H

#include <stdint.h>

typedef struct {
	char xbee_name[20];
	//char padding[4]; // MUST be 32-bit word aligned
} eeprom_t;

extern eeprom_t nvram;

void init_nvram();
void store_nvram();

#endif
