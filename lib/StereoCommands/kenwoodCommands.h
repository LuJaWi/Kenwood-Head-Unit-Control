#pragma once
#include <Arduino.h>

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

class KenwoodControl{
	public:
		KenwoodControl() : radio_pin(radio_pin) {};
		void volume_up();
		void volume_down();
		void play_pause();
		void next_track();
		void previous_track();

	private:
		const float NEC_UNIT = 562.5;
		CommandCodes* command_codes;
		byte address = 0xB9;
		void set_pin(uint8_t set_high);
		void send_byte(byte data);
		void nec_1();
		void nec_0();
		void transmission_start();
		void send_comand(CommandCodes command);
		volatile uint8_t* radio_pin_register;
		volatile uint8_t* radio_pin_port;
		int radio_pin;
		uint8_t radio_pin_bit;
};
