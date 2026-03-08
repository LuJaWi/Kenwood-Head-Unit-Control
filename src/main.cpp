#include "header.h"

#define ROTARY_CLK_PIN 6 // D6
#define ROTARY_DT_PIN 7  // D7
#define ROTARY_SW_PIN 5

#define RADIO_WIRE_PIN 4  // Pin to be wired to headunit


void setup()
{
    Serial.begin(9600);
}

RotaryKnob rotary_knob = RotaryKnob(ROTARY_CLK_PIN, ROTARY_DT_PIN, ROTARY_SW_PIN);
StereoCommand stereo_control = StereoCommand(RADIO_WIRE_PIN);
int rotation = 0;

void loop()
{
    rotation = rotary_knob.checkRotation();
    if (rotation == -1){
        stereo_control.VolumeDown();
    }
    else if (rotation == 1) {
        stereo_control.VolumeUp();
    };
}