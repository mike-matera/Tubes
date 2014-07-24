#include "ColorUtil.h"
#include <Arduino.h>

namespace mike {

float Color::distance(const HSV &a, const HSV &b) {
	HSV diff = a - b;
	return diff.hue * (1 - diff.saturation) * (1 - diff.value);
}

HSV operator+ (const HSV& a, const HSV &b) {
	float h,s,v;
	h = a.hue + b.hue;
	if (h < 0) {
		h = 6 + h;
	} else if (h > 6) {
		h = h - 6;
	}

	s = a.saturation + b.saturation;
	s = fmin(1,s);
	s = fmax(0,s);

	v = a.value + b.value;
	v = fmin(1,v);
	v = fmax(0,v);

	return HSV(h,s,v);
}


HSV operator-(const HSV &s, const HSV &e) {
	float h_step = e.hue - s.hue;
	if (abs(h_step) > 3) {
		if (h_step < 0) {
			h_step = 3-fmod(abs(h_step),3);
		}else{
			h_step = fmod(h_step,3)-3;
		}
	}
	return HSV(h_step, e.saturation-s.saturation, e.value-s.value);
}

HSV operator/(const HSV &a, const int b) {
	return HSV(a.hue/b, a.saturation/b, a.value/b);
}

// r,g,b values are from 0 to 1
// h = [0,6], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
HSV Color::RGBtoHSV( const RGB &c )
{
	float min, max, delta;
	HSV rval;
	min = fmin(c.red, fmin(c.green, c.blue));
	max = fmax(c.red, fmax(c.green, c.blue));

	rval.value = max;
	delta = max - min;
	if( max != 0 )
		rval.saturation = delta / max;
	else {
		// r = g = b = 0		// s = 0, v is undefined
		return HSV();
	}
	if( c.red == max )
		rval.hue = ( c.green - c.blue ) / delta;		// between yellow & magenta
	else if( c.green == max )
		rval.hue = 2 + ( c.blue - c.red ) / delta;	// between cyan & yellow
	else
		rval.hue = 4 + ( c.red - c.green ) / delta;	// between magenta & cyan

	if( rval.hue < 0 )
		rval.hue += 6;

	return rval;
}

RGB Color::HSVtoRGB(const HSV &c)
{
	int i;
	float f, p, q, t;
	if( c.saturation == 0 ) {
		return RGB(c.value, c.value, c.value);
	}
	i = floor( c.hue );
	f = c.hue - i;			// factorial part of h
	p = c.value * ( 1 - c.saturation );
	q = c.value * ( 1 - c.saturation * f );
	t = c.value * ( 1 - c.saturation * ( 1 - f ) );
	switch( i ) {
		case 0:
			return RGB(c.value,t,p);
		case 1:
			return RGB(q,c.value,p);
		case 2:
			return RGB(p,c.value,t);
		case 3:
			return RGB(p,q,c.value);
		case 4:
			return RGB(t,p,c.value);
		default:		// case 5:
			return RGB(c.value,p,q);
	}
}
}
