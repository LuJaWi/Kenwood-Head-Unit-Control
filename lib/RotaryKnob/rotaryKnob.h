#pragma once

class RotaryKnob {
    public:
        RotaryKnob(int clk_pin, int dt_pin, int sw_pin);
        int checkRotation();

    private:
        int clk_pin;
        int dt_pin;
        int sw_pin;
        int clk_last_state;
        int dt_last_state;
        unsigned long last_change_time;
        int last_rotation;
        static const unsigned long debounce_delay = 50; // milliseconds
        static const unsigned long direction_change_delay = 100; // milliseconds
};