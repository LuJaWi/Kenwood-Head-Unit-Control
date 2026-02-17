#include "rotaryKnob.h"
#include <Arduino.h>

RotaryKnob::RotaryKnob(int clk_pin, int dt_pin, int sw_pin) : clk_pin(clk_pin), dt_pin(dt_pin), sw_pin(sw_pin), last_change_time(0) {
    pinMode(clk_pin, INPUT);
    pinMode(dt_pin, INPUT);
    clk_last_state = digitalRead(clk_pin);
} 

int RotaryKnob::checkRotation(){
    unsigned long current_time = millis();
    int current_state = digitalRead(clk_pin); // Reads the "current" state of the outputA
    int rotation = 0;
    
    // If the previous and the current state of the outputA are different, that means a Pulse has occured
    if (current_state != clk_last_state)
    {
        // Check if enough time has passed since the last change (debounce)
        if (current_time - last_change_time > debounce_delay)
        {
            // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
            if (digitalRead(dt_pin) != current_state)
            {
                rotation = 1;
            }
            else
            {
                rotation = -1;
            }
            
            // Check for direction change bounce BEFORE updating timestamps
            unsigned long time_since_last_rotation = current_time - last_change_time;
            bool is_direction_change = (last_rotation != rotation && last_rotation != 0);
            

            
            // Filter out direction changes that happen too quickly (likely bounce)
            if (is_direction_change && time_since_last_rotation < direction_change_delay) {
                return 0;
            }
            clk_last_state = current_state;
            last_change_time = current_time;
            last_rotation = rotation;
        }
    }
    return rotation;
}