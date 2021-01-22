/*
 * Flames.cpp
 *
 */

#include "Flames.h"
#include "ColorUtil.h"

Flames::Flames() {
    random16_add_entropy(random());
}

Flames::~Flames() {
}

int Flames::render(hsv_buffer leds) {
    return 50000;
}

int Flames::render(rgb_buffer leds) {
    // Step 1.  Cool down every cell a little
    for (int i = 0; i < nLEDs; i++)
        _heat[i] = qsub8(_heat[i],  random8(0, ((_cooling * 10) / nLEDs) + 2));

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int i = nLEDs - 3; i > 0; i--)
        _heat[i] = (_heat[i - 1] + _heat[i - 2] + _heat[i - 2] ) / 3;

    // Step 3.  Randomly ignite new 'sparks' of _heat near the bottom
    for (int i = 0; i < 15; i++) {
        random16_add_entropy(random());
        if (random8() < _sparking)
        {
            int y = random8(0, 30);
            _heat[y] = qadd8(_heat[y], random8(120, 256));
            _heat[y + 1] = _heat[y];
        }
    }

    // Step 4.  Map from _heat cells to LED colors
    for (int j = 0; j < nLEDs; j++)
        leds[j] = heatColor(_heat[j]);

    return 40;
}

// CRGB HeatColor( uint8_t temperature)
// [to be included in the forthcoming FastLED v2.1]
//
// Approximates a 'black body radiation' spectrum for
// a given '_heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to maxBrightness (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's extremely fast and small.
//
// On AVR/Arduino, this typically takes around 70 bytes of program memory,
// versus 768 bytes for a full 256-entry RGB lookup table.

CRGB Flames::heatColor(uint8_t temperature)
{
    CRGB heatcolor = CRGB(0, 0, 0);

    if (temperature <= 50)
        return heatcolor;

    // Scale '_heat' down from 0-maxBrightness to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video(temperature, 192);

    // calculate a value that ramps up from
    // zero to maxBrightness in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
    if (heatramp > maxBrightness)
        heatramp = maxBrightness;

    // now figure out which third of the spectrum we're in:
    if (t192 & 0x80)
    {
        // we're in the hottest third
        //heatcolor.r = maxBrightness; // full red
        //heatcolor.g = maxBrightness; // full green
        //heatcolor.b = heatramp; // ramp up blue
        heatcolor.r = 0;
        heatcolor.b = maxBrightness;
        heatcolor.g = maxBrightness - heatramp;
    }
    else if(t192 & 0x40)
    {
        // we're in the middle third
        //heatcolor.r = maxBrightness; // full red
        //heatcolor.g = heatramp; // ramp up green
        //heatcolor.b = 0; // no blue
        heatcolor.r = maxBrightness - heatramp;
        heatcolor.b = maxBrightness;
        heatcolor.g = maxBrightness - heatramp;
    }
    else
    {
        // we're in the coolest third
        //heatcolor.r = heatramp; // ramp up red
        //heatcolor.g = 0; // no green
        //heatcolor.b = 0; // no blue
        heatcolor.r = heatramp;
        if (heatcolor.r > (maxBrightness/3))
            heatcolor.r = maxBrightness/3;
        heatcolor.b = maxBrightness/3;
        heatcolor.g = maxBrightness/3;
    }


    return heatcolor;
}
