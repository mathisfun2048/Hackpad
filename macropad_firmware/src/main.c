#include "pico/stdlib.h"
#include "tusb.h"
#include "config.h"
#include "matrix.h"
#include "encoder.h"
#include "oled.h"
#include "usb_hid.h"

// Layer definitions
typedef enum {
    LAYER_WASD = 0,
    LAYER_ARROWS = 1
} layer_t;

static layer_t current_layer = LAYER_WASD;

// Rotation mapping: physical key index -> logical key index (90° CCW)
static const uint8_t key_rotation[9] = {
    6,  // Physical key 0 (1) -> logical 6 (7)
    3,  // Physical key 1 (2) -> logical 3 (4)
    0,  // Physical key 2 (3) -> logical 0 (1)
    7,  // Physical key 3 (4) -> logical 7 (8)
    4,  // Physical key 4 (5) -> logical 4 (5)
    1,  // Physical key 5 (6) -> logical 1 (2)
    8,  // Physical key 6 (7) -> logical 8 (9)
    5,  // Physical key 7 (8) -> logical 5 (6)
    2   // Physical key 8 (9) -> logical 2 (3)
};

// Key mappings for each layer
// WASD Layer
static const struct {
    uint8_t keycode;
    uint8_t modifiers;
} layer_wasd[9] = {
    {HID_KEY_ESCAPE, 0},           // Key 1
    {HID_KEY_W, 0},                // Key 2
    {HID_KEY_TAB, 0},              // Key 3
    {HID_KEY_A, 0},                // Key 4
    {HID_KEY_S, 0},                // Key 5
    {HID_KEY_D, 0},                // Key 6
    {HID_KEY_LSHIFT, 0},           // Key 7 (Shift key)
    {0, 0},                        // Key 8 (Toggle - no action)
    {HID_KEY_SPACE, 0}             // Key 9 (Space)
};

// Arrow Keys Layer
static const struct {
    uint8_t keycode;
    uint8_t modifiers;
} layer_arrows[9] = {
    {HID_KEY_ESCAPE, 0},           // Key 1
    {0x52, 0},                     // Key 2 (Up Arrow)
    {HID_KEY_TAB, 0},              // Key 3
    {0x50, 0},                     // Key 4 (Left Arrow)
    {0x51, 0},                     // Key 5 (Down Arrow)
    {0x4F, 0},                     // Key 6 (Right Arrow)
    {HID_KEY_LSHIFT, 0},           // Key 7 (Shift key)
    {0, 0},                        // Key 8 (Toggle - no action)
    {HID_KEY_SPACE, 0}             // Key 9 (Space)
};

// Display labels for each layer
static const char* wasd_labels[9] = {
    "Esc", "W", "Tab",
    "A", "S", "D",
    "SH", "", "SP"
};

static const char* arrows_labels[9] = {
    "Esc", "Up", "Tab",
    "Left", "Down", "Rght",
    "SH", "", "SP"
};

void update_display(void) {
    if (current_layer == LAYER_WASD) {
        oled_draw_grid(wasd_labels, "WASD MODE");
    } else {
        oled_draw_grid(arrows_labels, "ARROW MODE");
    }
}

void handle_key_press(uint8_t key_index) {
    // Key 9 (index 8) toggles layers
    if (key_index == KEY_TOGGLE) {
        current_layer = (current_layer == LAYER_WASD) ? LAYER_ARROWS : LAYER_WASD;
        update_display();
        return;
    }
    
    if (current_layer == LAYER_WASD) {
        usb_hid_send_key(layer_wasd[key_index].keycode, layer_wasd[key_index].modifiers);
    } else {
        usb_hid_send_key(layer_arrows[key_index].keycode, layer_arrows[key_index].modifiers);
    }
}

void handle_encoder(int8_t delta) {
    if (delta == 0) return;
    
    // Weapon/item switching with left/right arrows
    if (delta > 0) {
        usb_hid_send_key(0x4F, 0);  // Right arrow
    } else {
        usb_hid_send_key(0x50, 0);  // Left arrow
    }
}

int main() {
    // Initialize hardware
    stdio_init_all();
    sleep_ms(1000);  // Wait for USB to enumerate
    
    matrix_init();
    encoder_init();
    usb_hid_init();
    
    if (!oled_init()) {
        // OLED failed, but continue anyway
    }
    
    update_display();
    
    uint16_t last_matrix_state = 0;
    
    while (1) {
        // Update USB
        tud_task();
        
        // Scan matrix
        uint16_t matrix_state = matrix_scan();
        
        // Check for key presses
        for (uint8_t i = 0; i < 9; i++) {
            if (matrix_key_pressed(i)) {
                uint8_t logical_key = key_rotation[i];  // Apply rotation
                handle_key_press(logical_key);
            }
            if (matrix_key_released(i)) {
                usb_hid_release_all();
            }
        }
        
        // Check encoder
        int8_t encoder_delta = encoder_get_delta();
        if (encoder_delta != 0) {
            handle_encoder(encoder_delta);
        }
        
        sleep_ms(1);
    }
    
    return 0;
}
