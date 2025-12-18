#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

// OLED display constants
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDRESS 0x3C

// Initialize OLED
bool oled_init(void);

// Clear display
void oled_clear(void);

// Display text at position (x in pixels, y in rows 0-3)
void oled_text(uint8_t x, uint8_t y, const char* str);

// Update display buffer to screen
void oled_update(void);

// Draw a 3x3 grid showing key labels
void oled_draw_grid(const char* keys[9], const char* title);

#endif // OLED_H
