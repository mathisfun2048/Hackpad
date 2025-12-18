#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Board specific configuration
#define CFG_TUSB_RHPORT0_MODE      OPT_MODE_DEVICE

// CFG_TUSB_DEBUG is defined by compiler in DEBUG build
#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

// USB Device Configuration
#define CFG_TUD_ENDPOINT0_SIZE    64

// Device class configuration
#define CFG_TUD_HID               2  // Keyboard + Consumer Control
#define CFG_TUD_CDC               0
#define CFG_TUD_MSC               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

// HID buffer size
#define CFG_TUD_HID_EP_BUFSIZE    16

#ifdef __cplusplus
}
#endif

#endif // TUSB_CONFIG_H
