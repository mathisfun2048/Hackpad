#include "usb_hid.h"
#include "pico/stdlib.h"
#include "tusb.h"

void usb_hid_init(void) {
    tusb_init();
}

void usb_hid_send_key(uint8_t keycode, uint8_t modifiers) {
    if (!tud_hid_ready()) return;
    
    uint8_t report[8] = {0};
    report[0] = modifiers;
    report[2] = keycode;
    
    tud_hid_keyboard_report(1, modifiers, &report[2]);
    sleep_ms(10);
}

void usb_hid_send_consumer(uint16_t code) {
    if (!tud_hid_ready()) return;
    
    tud_hid_report(2, &code, sizeof(code));
    sleep_ms(10);
    
    // Release
    uint16_t zero = 0;
    tud_hid_report(2, &zero, sizeof(zero));
}

void usb_hid_release_all(void) {
    if (!tud_hid_ready()) return;
    
    uint8_t report[8] = {0};
    tud_hid_keyboard_report(1, 0, &report[2]);
}

// TinyUSB callbacks
extern uint8_t const desc_hid_report[];

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    return desc_hid_report;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
}
