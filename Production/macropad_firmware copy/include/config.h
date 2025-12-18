#ifndef CONFIG_H
#define CONFIG_H

// Matrix pin definitions (GPIO numbers for XIAO RP2040)
#define COL1_PIN 0   // D6 = GPIO0
#define COL2_PIN 1   // D7 = GPIO1
#define COL3_PIN 2   // D8 = GPIO2

#define ROW1_PIN 3   // D10 = GPIO3
#define ROW2_PIN 4   // D9 = GPIO4
#define ROW3_PIN 6   // D4 = GPIO6

// Rotary encoder pins
#define ENCODER_A_PIN 26  // D0 = GPIO26
#define ENCODER_B_PIN 27  // D1 = GPIO27

// OLED pins (software I2C)
#define OLED_SCL_PIN 29   // D3 = GPIO29
#define OLED_SDA_PIN 7    // D5 = GPIO7

// Matrix configuration
#define NUM_ROWS 3
#define NUM_COLS 3
#define DEBOUNCE_MS 5

// Key mapping to 1-9
// Key layout:
//  1  2  3
//  4  5  6
//  7  8  9
#define KEY_TOGGLE 7  // Key 8 (index 7) toggles layers

#endif // CONFIG_H
