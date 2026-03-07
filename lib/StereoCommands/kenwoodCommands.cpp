#include "kenwoodCommands.h"
#include <util/delay.h>
#include <Arduino.h>

KenwoodControl::KenwoodControl(int radio_pin)
{
    radio_pin_register = portModeRegister(digitalPinToPort(radio_pin));
    radio_pin_port = portOutputRegister(digitalPinToPort(radio_pin));
}

void KenwoodControl::volume_up()
{
    send_comand(CMD_VOLUME_UP);
}

void KenwoodControl::volume_down()
{
    send_comand(CMD_VOLUME_DOWN);
}

void KenwoodControl::play_pause()
{
    send_comand(CMD_PLAY_PAUSE);
}

void KenwoodControl::next_track()
{
    send_comand(CMD_TRACK_NEXT);
}

void KenwoodControl::previous_track()
{
    send_comand(CMD_TRACK_BACK);
}

// Lines below are logic for building the commands
// Used https://init6.pomorze.pl/projects/kenwood_ford/
// as well as https://collindewey.net/articles/adding-swc-to-kenwood-for-cheap-using-arduino/
// for reference

void KenwoodControl::nec_1()
{
    set_pin(HIGH);
    _delay_us(NEC_UNIT);
    set_pin(LOW);
    _delay_us(NEC_UNIT * 3);
}

void KenwoodControl::nec_0()
{
    set_pin(HIGH);
    _delay_us(NEC_UNIT);
    set_pin(LOW);
    _delay_us(NEC_UNIT);
}

// Set the pin to a high or low state
void KenwoodControl::set_pin(uint8_t set_high)
{
    if (set_high)
    {
        *radio_pin_register &= ~radio_pin_bit;
        *radio_pin_port &= ~radio_pin_bit;
    }
    else
    {
        *radio_pin_port &= ~radio_pin_bit;
        *radio_pin_register |= radio_pin_bit;
    }
}

void KenwoodControl::transmission_start()
{
    // Transmission Start Signal
    set_pin(HIGH);
    _delay_us(NEC_UNIT * 16);
    set_pin(LOW);
    _delay_us(NEC_UNIT * 8);
    send_byte(address);
    send_byte(~address);
}

void KenwoodControl::send_comand(CommandCodes command)
{
    //
    transmission_start();
    // Send command
    send_byte(command);
    // Send inverse of command
    send_byte(~command);
    // End transmission with a single burst
    nec_0();
}

void KenwoodControl::send_byte(byte data)
{
    for (byte i = 0; i < 8; i++)
    {
        byte bit = bitRead(data, i);
        if (bit)
        {
            nec_1();
        }
        else
        {
            nec_0();
        }
    }
}
