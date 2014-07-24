/*
 * CellularAuto.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: fatboy
 */
#include "CellularAuto.h"

void CellularAuto::Advance(int rule) {

	INT_TYPE a, b, c, carry=0;

	for (int i=0; i<words; i++) {
		a = (state[i] >> 1) | carry;
		b = state[i];
		c = (state[i] << 1) | ((state[i+1] & msb_mask) >> (INT_BITS - 1));

		state[i] = 0;

		if (rule & 0x80) {
			state[i] |=  a & b & c;
		}
		if (rule & 0x40) {
			state[i] |=  a & b & ~c;
		}
		if (rule & 0x20) {
			state[i] |=  a & ~b & c;
		}
		if (rule & 0x10) {
			state[i] |=  a & ~b & ~c;
		}
		if (rule & 0x8) {
			state[i] |= ~a & b & c;
		}
		if (rule & 0x4) {
			state[i] |= ~a & b & ~c;
		}
		if (rule & 0x2) {
			state[i] |= ~a & ~b & c;
		}
		if (rule & 0x1) {
			state[i] |= ~a & ~b & ~c;
		}

		carry = (state[i] & 0x1) << (INT_BITS - 1);
	}
}
