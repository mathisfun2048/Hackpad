#include "matrix.h"
#include "config.h"
#include "pico/stdlib.h"
#include <string.h>

static const uint8_t col_pins[NUM_COLS] = {COL1_PIN, COL2_PIN, COL3_PIN};
static const uint8_t row_pins[NUM_ROWS] = {ROW1_PIN, ROW2_PIN, ROW3_PIN};

static uint16_t current_state = 0;
static uint16_t previous_state = 0;
static uint32_t debounce_time[NUM_ROWS * NUM_COLS] = {0};

void matrix_init(void) {
    // Initialize column pins as outputs (default low)
    for (int i = 0; i < NUM_COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_OUT);
        gpio_put(col_pins[i], 0);
    }
    
    // Initialize row pins as inputs with pull-down
    for (int i = 0; i < NUM_ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_IN);
        gpio_pull_down(row_pins[i]);
    }
}

uint16_t matrix_scan(void) {
    previous_state = current_state;
    uint16_t new_state = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    
    // Scan each column
    for (int col = 0; col < NUM_COLS; col++) {
        // Set current column high
        gpio_put(col_pins[col], 1);
        sleep_us(10); // Small delay for signal to stabilize
        
        // Read all rows
        for (int row = 0; row < NUM_ROWS; row++) {
            uint8_t key_index = row * NUM_COLS + col;
            bool pressed = gpio_get(row_pins[row]);
            
            // Debouncing: only update state if enough time has passed
            if (now - debounce_time[key_index] > DEBOUNCE_MS) {
                bool was_pressed = (current_state >> key_index) & 1;
                
                if (pressed != was_pressed) {
                    debounce_time[key_index] = now;
                    if (pressed) {
                        new_state |= (1 << key_index);
                    }
                } else if (pressed) {
                    new_state |= (1 << key_index);
                }
            } else {
                // Keep previous state during debounce
                if ((current_state >> key_index) & 1) {
                    new_state |= (1 << key_index);
                }
            }
        }
        
        // Set column back to low
        gpio_put(col_pins[col], 0);
    }
    
    current_state = new_state;
    return current_state;
}

bool matrix_key_pressed(uint8_t key_index) {
    uint16_t changed = current_state ^ previous_state;
    return (changed & (1 << key_index)) && (current_state & (1 << key_index));
}

bool matrix_key_released(uint8_t key_index) {
    uint16_t changed = current_state ^ previous_state;
    return (changed & (1 << key_index)) && !(current_state & (1 << key_index));
}
