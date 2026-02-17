#pragma once

enum CommandCodes {
	CMD_NONE = 0xFF,
	CMD_TRACK_BACK = 0xA,
	CMD_TRACK_NEXT = 0xB,
	CMD_REVERSE = 0xC,
	CMD_FAST_FORWARD = 0xD,
	CMD_PLAY_PAUSE = 0xE,
	CMD_SOURCE = 0x13,
	CMD_VOLUME_UP = 0x14,
	CMD_VOLUME_DOWN = 0x15,
	CMD_MUTE = 0x16
};

// From https://init6.pomorze.pl/projects/kenwood_ford/:
//  NEC protocol transmission data info:

//     Time of a logical bit is 562.5µs
//     Logical 1 is 562.5µs burst and 1.6875ms space
//     Logical 0 is 562.5µs burst and 562.5µs space
//     Transmitting a message frame takes 67.5ms
//     Addresses and commands are sent LSB first


#define NEC_ADDRESS 0xB9 // Kenwood
#define NEC_UNIT 562.5
#define NUM_BUTTONS 7 // OPEN included
#define IN_RANGE_OHM(O, L, T) ((O) >= (((long)(L) * 1023) / ((L) + DIV_RESISTANCE)) && (O) <= (((long)(T) * 1023) / ((T) + DIV_RESISTANCE)))
#define RADIO_PIN_REG portModeRegister(digitalPinToPort(RADIO_PIN))
#define RADIO_PIN_PORT portOutputRegister(digitalPinToPort(RADIO_PIN))
#define RADIO_PIN_BIT digitalPinToBitMask(RADIO_PIN)
