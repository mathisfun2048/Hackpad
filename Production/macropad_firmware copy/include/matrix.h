#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

// Initialize matrix scanning
void matrix_init(void);

// Scan matrix and return bitmask of pressed keys (bits 0-8 for keys 1-9)
uint16_t matrix_scan(void);

// Check if a specific key changed state (pressed or released)
bool matrix_key_pressed(uint8_t key_index);
bool matrix_key_released(uint8_t key_index);

#endif // MATRIX_H
