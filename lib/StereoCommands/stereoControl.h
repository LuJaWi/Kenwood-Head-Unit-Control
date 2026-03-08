#pragma once
#include "kenwoodCommands.h"


class StereoCommand{
    public:
        StereoCommand(int radio_pin);
        void VolumeUp();
        void VolumeDown();
        void PlayPause();
        void NextTrack();
        void PreviousTrack();
    
    private:
        int radio_pin;
        KenwoodControl stereo_control;
};