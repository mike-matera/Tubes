/*
 * ColorUtil.h
 *
 *  Created on: Jun 23, 2013
 *      Author: maximus
 */

#ifndef COLORUTIL_H_
#define COLORUTIL_H_

#include <Arduino.h>

namespace mike {

class RGB;
class HSV;

class Color {
public:
	static RGB HSVtoRGB(const HSV &val);
	static HSV RGBtoHSV(const RGB &val);
	static float distance(const HSV &a, const HSV &b);
};

class RGB : Color {
public:
	float red;
	float green;
	float blue;

public:
	RGB() {
		red = 0; green = 0; blue = 0;
	}

	RGB(float r, float g, float b) {
		red = r; green = g; blue = b;
	}

	RGB(uint32_t value) {
		int r,g,b;
		r = (value & 0xff0000) >> 16;
		g = (value & 0xff00) >> 8;
		b = (value & 0xff) >> 0;
		red = ((float) r) / 255.0;
		green = ((float) g) / 255.0;
		blue = ((float) b) / 255.0;
	}

	RGB(const RGB &other) {
		red = other.red;
		green = other.green;
		blue = other.blue;
	}

	uint32_t toColor() const {
		uint32_t rval = 0;
		rval |= (int) (255 * red) & 0xff;
		rval = rval << 8;
		rval |= (int) (255 * green) & 0xff;
		rval = rval << 8;
		rval |= (int) (255 * blue) & 0xff;
		return rval;
	}

	RGB &operator= (const RGB &other) {
		red = other.red;
		green = other.green;
		blue = other.blue;
		return *this;
	}
};

class HSV : Color {
public:
	float hue;
	float saturation;
	float value;

public:
	HSV () {
		hue = 0; saturation = 0; value = 0;
	}

	HSV (const HSV &other) {
		hue = other.hue;
		saturation = other.saturation;
		value = other.value;
	}

	HSV(float h, float s, float v) {
		hue = h;
		saturation = s;
		value = v;
	}

	HSV(uint32_t color) {
		HSV v = RGBtoHSV(RGB(color));
		hue = v.hue;
		value = v.value;
		saturation = v.saturation;
	}

	uint32_t toColor() const {
		RGB r = HSVtoRGB(*this);
		return r.toColor();
	}

	const HSV &operator= (const HSV &other) {
		hue = other.hue;
		saturation = other.saturation;
		value = other.value;
		return *this;
	}

	void invert() {
		hue = fmod(hue + 3.0, 6);
	}
};

HSV operator+ (const HSV&a, const HSV &b);
HSV operator- (const HSV&a, const HSV &b);
HSV operator/ (const HSV&a, const int b);

}

#endif /* COLORUTIL_H_ */
