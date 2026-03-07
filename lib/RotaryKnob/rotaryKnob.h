#pragma once

class RotaryKnob {
    public:
        RotaryKnob(int clk_pin, int dt_pin, int sw_pin);
        int checkRotation();
        bool checkPress();

    private:
        int clk_pin;
        int dt_pin;
        int sw_pin;
        int clk_last_state;
        int dt_last_state;
        unsigned long last_check_time = 0;
        int last_rotation;
        static const unsigned long debounce_delay = 5; // milliseconds
        // static const unsigned long direction_change_delay = 1000; // milliseconds
        int turn_counter = 0;
        bool idle_state = false;
};