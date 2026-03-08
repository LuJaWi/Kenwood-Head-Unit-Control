#include "rotaryKnob.h"
#include <Arduino.h>

RotaryKnob::RotaryKnob(int clk_pin, int dt_pin, int sw_pin) : clk_pin(clk_pin), dt_pin(dt_pin), sw_pin(sw_pin), last_check_time(0)
{
    pinMode(clk_pin, INPUT);
    pinMode(dt_pin, INPUT);
    pinMode(sw_pin, INPUT_PULLUP);
    clk_last_state = digitalRead(clk_pin);
}

int RotaryKnob::checkRotation()
{
    int rotation = 0;
    unsigned long current_time = millis();
    int clk_current_state = digitalRead(clk_pin);
    int dt_current_state = digitalRead(dt_pin);
    rotation = 0;
    // To avoid double counting rotations, 
    // we wait for the knob to hit the idle state (both 1)
    // before saying it has rotated.
    if (dt_current_state > clk_current_state)
    {
        if (idle_state && !(current_time - last_check_time < debounce_delay))
        {
            rotation = 1;
            last_rotation = rotation;
            idle_state = false;
        }
    }
    else if (dt_current_state < clk_current_state)
    {
        if (idle_state && !(current_time - last_check_time < debounce_delay))
        {
            rotation = -1;
            last_rotation = rotation;
            idle_state = false;
        }
    }
    else if (clk_current_state == 0) // When both knobs read zero, it's between detents
    { 
        rotation = 0;
        idle_state = false;
        last_check_time = millis();
    }
    else
    {
        idle_state = true;
        rotation = 0;
        last_rotation = rotation;
    }
    // }
    clk_last_state = clk_current_state;
    dt_last_state = dt_current_state;
    turn_counter += rotation;
    return rotation;
}

bool RotaryKnob::checkPress()
{
    unsigned long current_time = millis();
    if (current_time - last_check_time > debounce_delay)
    {
        return digitalRead(sw_pin) == LOW;
    }
}
