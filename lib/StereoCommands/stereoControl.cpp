#include "stereoControl.h"
#include <Arduino.h>


StereoCommand::StereoCommand() {};

void StereoCommand::VolumeUp()
{
    Serial.println("Volume Up");
}

void StereoCommand::VolumeDown()
{
    Serial.println("Volume Down");
}

void StereoCommand::PlayPause()
{
    Serial.println("Play/Pause");
}