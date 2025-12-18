#ifndef USB_HID_H
#define USB_HID_H

#include <stdint.h>

// HID keycodes (subset)
#define HID_KEY_A 0x04
#define HID_KEY_C 0x06
#define HID_KEY_D 0x07
#define HID_KEY_S 0x16
#define HID_KEY_V 0x19
#define HID_KEY_W 0x1A
#define HID_KEY_X 0x1B
#define HID_KEY_Z 0x1D
#define HID_KEY_ESCAPE 0x29
#define HID_KEY_SPACE 0x2C
#define HID_KEY_TAB 0x2B
#define HID_KEY_LSHIFT 0xE1  // Left shift key

// Modifier keys
#define HID_MOD_LCTRL 0x01
#define HID_MOD_LSHIFT 0x02
#define HID_MOD_LALT 0x04

// Consumer control codes
#define HID_CONSUMER_VOLUME_UP 0xE9
#define HID_CONSUMER_VOLUME_DOWN 0xEA
#define HID_CONSUMER_MUTE 0xE2

// Initialize USB HID
void usb_hid_init(void);

// Send keyboard report
void usb_hid_send_key(uint8_t keycode, uint8_t modifiers);

// Send consumer control (media keys)
void usb_hid_send_consumer(uint16_t code);

// Release all keys
void usb_hid_release_all(void);

#endif // USB_HID_H
