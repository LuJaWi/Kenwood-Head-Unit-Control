#include "header.h"

#define ROTARY_SW_PIN 5   // D5
#define ROTARY_CLK_PIN 6  // D6
#define ROTARY_DT_PIN 7   // D7

#define RADIO_WIRE_PIN 2  // Pin to be wired to headunit


void setup()
{
    Serial.begin(9600);
}

RotaryKnob rotary_knob = RotaryKnob(ROTARY_CLK_PIN, ROTARY_DT_PIN, ROTARY_SW_PIN);
StereoCommand stereo_control = StereoCommand(RADIO_WIRE_PIN);

void loop()
{
    int rotation = rotary_knob.checkRotation();
    if (rotation == -1){
        stereo_control.VolumeDown();
    }
    else if (rotation == 1) {
        stereo_control.VolumeUp();
    };

    int press_count = rotary_knob.checkPressCount();
    if (press_count == 1)
    {
        stereo_control.PlayPause();
    }
    else if (press_count == 2)
    {
        stereo_control.NextTrack();
    }
    else if (press_count >= 3)
    {
        stereo_control.PreviousTrack();
    }
}