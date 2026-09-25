# Kenwood Head Unit Control

Control a Kenwood car stereo with a single rotary knob. An Arduino Nano reads a rotary encoder and sends commands to the head unit's **steering wheel remote input**, using the same NEC-style signal a factory steering wheel adapter would send.

| Knob action           | Stereo command   |
| --------------------- | ---------------- |
| Turn clockwise        | Volume up        |
| Turn counterclockwise | Volume down      |
| Press once            | Play / Pause     |
| Press twice           | Next track       |
| Press three times     | Previous track   |

---

## Hardware

### What you need

- **Arduino Nano** (ATmega328P, "new bootloader"). Clones work too.
- **Rotary encoder with push button**, such as the common KY-040 module
- **Kenwood head unit** with a steering wheel remote input. This is usually a 3.5 mm jack on the back labeled `REMOTE` or `STEERING WHEEL REMOTE INPUT`.
- **3.5 mm mono plug** (or a cut audio cable) to connect to that jack
- A **5 V supply** for the Nano, such as a 12 V to 5 V buck converter on switched accessory power, or a USB car adapter
- Hookup wire

### Wiring

| Arduino Nano pin | Connects to                                    |
| ---------------- | ---------------------------------------------- |
| `D2`             | Head unit remote input (tip of the 3.5 mm plug) |
| `D5`             | Encoder `SW` (push button)                     |
| `D6`             | Encoder `CLK`                                  |
| `D7`             | Encoder `DT`                                   |
| `5V`             | Encoder `+`                                    |
| `GND`            | Encoder `GND`, the sleeve of the 3.5 mm plug, and vehicle/head unit ground |

```
            Arduino Nano
          ┌──────────────┐
 Encoder  │              │
   SW ────┤ D5        D2 ├──── Remote input (tip)  ──┐
   CLK ───┤ D6           │                           │  Kenwood
   DT ────┤ D7           │                           │  head unit
   + ─────┤ 5V       GND ├──── Remote ground (sleeve)┘
   GND ───┤ GND          │
          └──────────────┘
```

> **Note:** The Arduino never drives the remote line to 5 V. For a logic "high", the pin is set to high-impedance input, and the head unit pulls the line up to its own ~3.3 V. For a logic "low", the pin pulls the line to ground. You don't need a level shifter, but the **Arduino and the head unit must share a common ground**.

To use different pins, change the `#define`s at the top of [`src/main.cpp`](src/main.cpp).

---

## Software setup

This project uses [PlatformIO](https://platformio.org/).

### Option A: VS Code (recommended)

1. Install [VS Code](https://code.visualstudio.com/).
2. Clone this repo and open the folder in VS Code:
   ```bash
   git clone https://github.com/LuJaWi/Kenwood-Head-Unit-Control.git
   code Kenwood-Head-Unit-Control
   ```
3. When prompted, install the recommended **PlatformIO IDE** extension.
4. Connect the Nano over USB. In the PlatformIO toolbar, click **Upload** (→).
5. Optional: click **Serial Monitor** (plug icon) at 9600 baud to watch the commands as they're sent.

### Option B: PlatformIO CLI

```bash
# Install PlatformIO Core (see https://docs.platformio.org/page/core/installation/index.html)
pip install platformio

git clone https://github.com/LuJaWi/Kenwood-Head-Unit-Control.git
cd Kenwood-Head-Unit-Control

pio run                          # build
pio run --target upload          # flash the Nano
pio device monitor -b 9600       # optional: view debug output
```

PlatformIO downloads the AVR toolchain and the library dependencies (see [`platformio.ini`](platformio.ini)) automatically on the first build.

### Troubleshooting uploads

- **Upload fails or times out:** some Nano clones use the old bootloader. In `platformio.ini`, change `board = nanoatmega328new` to `board = nanoatmega328` and try again.
- **Port not found:** many clones use a CH340 USB chip, which may need a [driver](https://www.wch-ic.com/downloads/CH341SER_ZIP.html) on some systems.

---

## Testing

1. Flash the Nano and open the serial monitor at 9600 baud.
2. Turn and press the knob. You should see `Volume Up`, `Volume Down`, `Play/Pause`, `Next Track` and `Previous Track` printed.
3. Connect the Nano to the head unit (signal **and** ground) and repeat. The stereo should respond.

If the serial output looks right but the stereo doesn't react, check the ground connection first, then check that the signal wire is on the tip of the remote jack.

---

## Project structure

```
├── src/main.cpp                      # Pin config and main loop: maps knob input to stereo commands
├── include/header.h                  # Shared includes
├── lib/
│   ├── RotaryKnob/                   # Encoder rotation + debounced single/double/triple press detection
│   └── StereoCommands/
│       ├── kenwoodCommands.*         # Low-level NEC-style protocol and Kenwood command codes
│       └── stereoControl.*           # High-level command wrapper (adds serial logging)
├── main_test.cpp                     # Standalone reference sketch for resistor-ladder steering wheel buttons (not built)
└── platformio.ini                    # Build configuration
```

### How it works

Kenwood head units accept the NEC infrared protocol over a wire instead of IR. Each command frame has:

1. A 9 ms low "leader" pulse and a 4.5 ms high gap
2. The address byte (`0xB9` for Kenwood) and its inverse
3. The command byte and its inverse
4. A final 562 µs stop pulse

Each bit is a 562 µs low pulse followed by a high gap: 562 µs for `0` or 1687 µs for `1`. Bytes are sent least significant bit first.

Supported command codes are listed in [`lib/StereoCommands/kenwoodCommands.h`](lib/StereoCommands/kenwoodCommands.h). The list includes extra codes (source, mute, fast forward, reverse) that you can map to knob actions.

### Adding or remapping actions

1. Add a method for the command in `KenwoodControl` (`kenwoodCommands.*`) that calls `send_command(CMD_...)`.
2. Wrap it in `StereoCommand` (`stereoControl.*`).
3. Call it from `loop()` in `src/main.cpp`.

---

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

## Acknowledgements

The code for this project was heavily inspired by [this project from Collin Dewey](https://collindewey.net/articles/adding-swc-to-kenwood-for-cheap-using-arduino/).

Some folks whose work I also referenced heavily:

- [Michal Babik](https://init6.pomorze.pl/projects/kenwood_ford/)
