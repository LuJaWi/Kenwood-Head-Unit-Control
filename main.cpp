#include <Arduino.h>
#include <TimerOne.h>

// Diagram ---------------------------------------------------------
// Logic Voltage (5V/3.3V)--------[resistor]----+----[Analog Read]
//                                    |
//                          [Steering Wheel Buttons]
//                                    |
//                                   GND

// Set pins & resistance used (Touch this)
#define RADIO_PIN 13 // Pin for Kenwood remote wire. Disconnect the internal LED if you use 13
#define READ_PIN A5 // Pin for wheel buttons
#define DIV_RESISTANCE 470 // Resistance of the resistor you're using (in Ohms)

// Don't touch this
#define NEC_ADDRESS 0xB9 // Kenwood
#define NEC_UNIT 562.5
#define NUM_BUTTONS 7 // OPEN included
#define IN_RANGE_OHM(O, L, T) ((O) >= (((long)(L) * 1023) / ((L) + DIV_RESISTANCE)) && (O) <= (((long)(T) * 1023) / ((T) + DIV_RESISTANCE)))
#define RADIO_PIN_REG portModeRegister(digitalPinToPort(RADIO_PIN))
#define RADIO_PIN_PORT portOutputRegister(digitalPinToPort(RADIO_PIN))
#define RADIO_PIN_BIT digitalPinToBitMask(RADIO_PIN)

// Get more from https://init6.pomorze.pl/projects/kenwood_ford/
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

// List from my repair manual
enum WheelButton {
	BUTTON_VOLUME_MINUS = 0,
	BUTTON_VOLUME_PLUS = 1,
	BUTTON_AUTO_SCAN = 2,
	BUTTON_PRESET = 3,
	BUTTON_MODE = 4,
	BUTTON_MUTE = 5,
	BUTTON_OPEN = 6
};

CommandCodes getCommand();
void loop();
void setup();
void handleButtons();
void repeat_heartbeat();
void send_command(CommandCodes command);
void radioWrite(int value, unsigned int delay);
void send_byte(byte data);

CommandCodes getCommand() {
	int hits[NUM_BUTTONS] = {0};

	// 100 reads is probably enough (Takes about 10ms)
	// Press the button for less than 5ms every 108ms for this to fail, I dare you.
	for (int i = 0; i < 100; i++) {
		int value = analogRead(READ_PIN);
		WheelButton button = BUTTON_OPEN;

		// Find resistance values in repair manual
		if (IN_RANGE_OHM(value, 51, 56)) button = BUTTON_VOLUME_MINUS;
		else if (IN_RANGE_OHM(value, 140, 154)) button = BUTTON_VOLUME_PLUS;
		else if (IN_RANGE_OHM(value, 286, 315)) button = BUTTON_AUTO_SCAN;
		else if (IN_RANGE_OHM(value, 534, 589)) button = BUTTON_PRESET;
		else if (IN_RANGE_OHM(value, 985, 1080)) button = BUTTON_MODE;
		else if (IN_RANGE_OHM(value, 1940, 2130)) button = BUTTON_MUTE;
		else if (IN_RANGE_OHM(value, 4800, 5290)) button = BUTTON_OPEN;

		hits[button]++;
	}

	// Most pressed button
	WheelButton command = BUTTON_OPEN;
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (hits[i] > hits[command]) {
			command = (WheelButton) i;
		}
	}
	
	// Map buttons to commands
	switch (command) {
		case BUTTON_VOLUME_MINUS: return CMD_VOLUME_DOWN;
		case BUTTON_VOLUME_PLUS: return CMD_VOLUME_UP;
		case BUTTON_AUTO_SCAN: return CMD_VOLUME_UP;
		case BUTTON_PRESET: return CMD_VOLUME_DOWN;
		case BUTTON_MODE: return CMD_MUTE;
		case BUTTON_MUTE: return CMD_MUTE;
		case BUTTON_OPEN: return CMD_NONE;
		default: return CMD_NONE;
	}
}

void loop() {}

void setup() {
	Timer1.initialize((unsigned long)NEC_UNIT * 192);
	Timer1.attachInterrupt(handleButtons);
}

void handleButtons() {
	Timer1.start(); // Renew timer

	static CommandCodes lastCommand = CMD_NONE;
	CommandCodes newCommand = getCommand();
	
	if (newCommand != CMD_NONE) {
		if (newCommand == lastCommand) {
			repeat_heartbeat();
		} else {
			send_command(newCommand);
		}
	}
	
	lastCommand = newCommand;
}

// https://www.sbprojects.net/knowledge/ir/necrepeat.png
void repeat_heartbeat() {
	radioWrite(LOW, NEC_UNIT * 16);
	radioWrite(HIGH, NEC_UNIT * 4);
	radioWrite(LOW, NEC_UNIT);
	radioWrite(HIGH, 0);
}

// https://www.sbprojects.net/knowledge/ir/nectrain.png
void send_command(CommandCodes command) {
	// Start frame
	radioWrite(LOW, NEC_UNIT * 16);
	radioWrite(HIGH, NEC_UNIT * 8);

	// Send address, address inverse, command, inverse command
	send_byte((byte) NEC_ADDRESS);
	send_byte((byte) ~NEC_ADDRESS);
	send_byte((byte) command);
	send_byte((byte) ~command);

	// End of frame
	radioWrite(LOW, NEC_UNIT);
	radioWrite(HIGH, 0);
}

void radioWrite(int value, unsigned int delay) {
	if (value == HIGH) { // Floating (The radio pulls the voltage to 3.3V, we don't want to drive 5V into it with a real HIGH)
		*RADIO_PIN_REG &= ~RADIO_PIN_BIT;
		*RADIO_PIN_PORT  &= ~RADIO_PIN_BIT;
	} else { // Pull low
		*RADIO_PIN_PORT  &= ~RADIO_PIN_BIT;
		*RADIO_PIN_REG |= RADIO_PIN_BIT;
	}
	delayMicroseconds(delay);
}

void send_byte(byte data) {
	for (byte i = 0; i < 8; i++) {
		byte bit = bitRead(data, i);
		radioWrite(LOW, NEC_UNIT);
		radioWrite(HIGH, bit ? NEC_UNIT * 3 : NEC_UNIT);
	}
}