#include "stereoControl.h"
#include <Arduino.h>


StereoCommand::StereoCommand(int radio_pin) : radio_pin(radio_pin), stereo_control(radio_pin) {
};

void StereoCommand::VolumeUp()
{
    Serial.println("Volume Up");
    stereo_control.volume_up();
}

void StereoCommand::VolumeDown()
{
    Serial.println("Volume Down");
    stereo_control.volume_down();
}

void StereoCommand::PlayPause()
{
    Serial.println("Play/Pause");
    stereo_control.play_pause();
}

void StereoCommand::NextTrack()
{
    Serial.println("Next Track");
    stereo_control.next_track();
}

void StereoCommand::PreviousTrack()
{
    Serial.println("Previous Track");
    stereo_control.previous_track();
}