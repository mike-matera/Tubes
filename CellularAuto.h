/*
 *
 *  Created on: Jul 10, 2011
 *      Author: fatboy
 */

#ifndef CELLULARAUTO_H_
#define CELLULARAUTO_H_

#include <Arduino.h>

#define INT_BITS     32
#define INT_TYPE     uint32_t

class CellularAuto {
private:
	INT_TYPE *state;
	INT_TYPE msb_mask;
	int words;

public:
	CellularAuto(int s) {
		words = (s/INT_BITS)+1;
		state = (INT_TYPE *) calloc(words+1, sizeof(INT_TYPE));
		msb_mask = 0x1 << (INT_BITS - 1);
	}

	~CellularAuto() {
		free(state);
	}

	void Reset() {
		memset(state, 0, words/sizeof(INT_TYPE));
	}

	void Set(int bit) {
		state[bit/INT_BITS] |= msb_mask >> (bit % INT_BITS);
	}

	bool Select(int bit) {
		return ((state[bit/INT_BITS] & (msb_mask >> (bit%INT_BITS))) != 0);
	}

	void Advance(int rule);
};

#endif
