#pragma once

class RotaryKnob {
    public:
        RotaryKnob(int clk_pin, int dt_pin, int sw_pin);
        int checkRotation();
        bool checkPress();
        int checkPressCount();

    private:
        int clk_pin;
        int dt_pin;
        int sw_pin;
        int clk_last_state;
        int dt_last_state;
        unsigned long last_knob_check_time = 0;
        unsigned long last_sw_check_time = 0;
        bool button_pressed_state = false;
        unsigned long last_multi_press_time = 0;
        int press_count = 0;
        int last_rotation;
        static const unsigned long knob_debounce_delay = 5; // milliseconds
        static const unsigned long button_debounce_delay = 50;
        static const unsigned long multi_press_window_ms = 400;
        // static const unsigned long direction_change_delay = 1000; // milliseconds
        int turn_counter = 0;
        bool idle_state = false;
};