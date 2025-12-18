# Macropad Firmware for XIAO RP2040

9-key macropad with rotary encoder and OLED display.

## Hardware
- XIAO RP2040 MCU
- 3x3 key matrix
- Rotary encoder (non-clickable)
- 32x128 OLED (SSD1306, software I2C on D3/D5)

## Features
- **2 Layers** (toggle with Key 9):
  - **Gameplay**: WASD + gaming shortcuts, encoder for weapon/item switching
  - **Shortcuts**: Copy/Paste/Save/Undo/Redo + volume controls, encoder for volume

## Build Instructions

### Prerequisites
- Pico SDK installed
- CMake and ARM GCC toolchain

### Setup
```bash
export PICO_SDK_PATH=/path/to/pico-sdk
mkdir build && cd build
cmake ..
make
```

### Flash
1. Hold BOOTSEL button on XIAO while plugging in USB
2. Copy `macropad.uf2` to the mounted drive
3. Device will reboot and appear as USB HID keyboard

## Key Layout

**Layer 1 - Gameplay:**
```
[Esc] [W]   [Tab]
[A]   [S]   [D]
[Ctrl][Shft][Spc]
```
Encoder: Left/Right arrows (weapon switching)

**Layer 2 - Shortcuts:**
```
[Copy][Paste][Undo]
[Cut] [Save] [Redo]
[Vol-][Mute][Vol+]
```
Encoder: Volume control

**Toggle:** Press Key 9 (bottom-right) to switch layers

## Troubleshooting

**OLED not working:**
- Check D3/D5 connections (software I2C, not hardware I2C pins)
- Verify OLED address is 0x3C

**Keys not registering:**
- Check diode orientation (col → row current flow)
- Verify pin definitions in `include/config.h`

**USB not recognized:**
- Try different USB cable
- Check USB descriptors in `src/usb_descriptors.c`

## Future Plans
- Game modes playable on OLED
- Configurable key mappings
- Additional layers
