#ifdef USE_USBDEVICE

#include "USBDevice.h"

#include "USBDevice_BSP_inc.h"

// @note(interrupt handler callback moved to `USBDevice_BSP_KLST_*_usbd_conf.c`) )

void usb_device_init() {
    MX_USB_DEVICE_Init();
}

extern USBD_HandleTypeDef hUsbDeviceFS;

// @TODO(move devices into individual classes + files)

/* --- MIDI ---------------------------------------------------------------- */

#if defined(USBDEVICE_MIDI)
#include "usbd_midi.h"
#include "usbd_midi_if.h"

void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    MIDI_note_on(note, velocity);
    USBD_UsrLog("note on");
}

void transmit_midi_note_off(const uint8_t channel, const uint8_t note) {
    MIDI_note_off(note, 0);
    USBD_UsrLog("note off");
}

void transmit_midi_control_change(const uint8_t channel, const uint8_t number, const uint8_t value) {
    MIDI_cc(channel, number, value);
    USBD_UsrLog("control change");
}
#endif  // USBDEVICE_MIDI

/* --- MOUSE + KEYBOARD ----------------------------------------------------- */

// #if defined(USBDEVICE_MOUSE_KEYBOARD)
#if defined(USBDEVICE_MOUSE)
#include "usbd_hid.h"

/* --- MOUSE ---------------------------------------------------------------- */
static uint8_t fButton = 0;

void transmit_mouse_moved(const int8_t x, const int8_t y) {
    typedef struct mouseHID_struct {
        uint8_t buttons;
        int8_t  x;
        int8_t  y;
        int8_t  wheel;
    } mouseHID_t;
    mouseHID_t mouseHID;
    mouseHID.buttons = fButton;
    mouseHID.x       = x;
    mouseHID.y       = y;
    mouseHID.wheel   = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)(&mouseHID), sizeof(struct mouseHID_struct));
}

void transmit_mouse_button(const uint8_t button) {
    typedef struct mouseHID_struct {
        uint8_t buttons;
        int8_t  x;
        int8_t  y;
        int8_t  wheel;
    } mouseHID_t;
    mouseHID_t mouseHID;
    mouseHID.buttons = button;
    mouseHID.x       = 0;
    mouseHID.y       = 0;
    mouseHID.wheel   = 0;
    fButton          = button;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)(&mouseHID), sizeof(struct mouseHID_struct));
}
#endif

#if defined(USBDEVICE_KEYBOARD)
#include "usbd_hid.h"

/* --- KEYBOARD ------------------------------------------------------------- */

#define SHIFT 0x80
const uint8_t _asciimap[128] =
    {
        0x00,  // NUL
        0x00,  // SOH
        0x00,  // STX
        0x00,  // ETX
        0x00,  // EOT
        0x00,  // ENQ
        0x00,  // ACK
        0x00,  // BEL
        0x2a,  // BS        Backspace
        0x2b,  // TAB        Tab
        0x28,  // LF        Enter
        0x00,  // VT
        0x00,  // FF
        0x00,  // CR
        0x00,  // SO
        0x00,  // SI
        0x00,  // DEL
        0x00,  // DC1
        0x00,  // DC2
        0x00,  // DC3
        0x00,  // DC4
        0x00,  // NAK
        0x00,  // SYN
        0x00,  // ETB
        0x00,  // CAN
        0x00,  // EM
        0x00,  // SUB
        0x00,  // ESC
        0x00,  // FS
        0x00,  // GS
        0x00,  // RS
        0x00,  // US

        0x2c,          //  ' '
        0x1e | SHIFT,  // !
        0x34 | SHIFT,  // "
        0x20 | SHIFT,  // #
        0x21 | SHIFT,  // $
        0x22 | SHIFT,  // %
        0x24 | SHIFT,  // &
        0x34,          // '
        0x26 | SHIFT,  // (
        0x27 | SHIFT,  // )
        0x25 | SHIFT,  // *
        0x2e | SHIFT,  // +
        0x36,          // ,
        0x2d,          // -
        0x37,          // .
        0x38,          // /
        0x27,          // 0
        0x1e,          // 1
        0x1f,          // 2
        0x20,          // 3
        0x21,          // 4
        0x22,          // 5
        0x23,          // 6
        0x24,          // 7
        0x25,          // 8
        0x26,          // 9
        0x33 | SHIFT,  // :
        0x33,          // ;
        0x36 | SHIFT,  // <
        0x2e,          // =
        0x37 | SHIFT,  // >
        0x38 | SHIFT,  // ?
        0x1f | SHIFT,  // @
        0x04 | SHIFT,  // A
        0x05 | SHIFT,  // B
        0x06 | SHIFT,  // C
        0x07 | SHIFT,  // D
        0x08 | SHIFT,  // E
        0x09 | SHIFT,  // F
        0x0a | SHIFT,  // G
        0x0b | SHIFT,  // H
        0x0c | SHIFT,  // I
        0x0d | SHIFT,  // J
        0x0e | SHIFT,  // K
        0x0f | SHIFT,  // L
        0x10 | SHIFT,  // M
        0x11 | SHIFT,  // N
        0x12 | SHIFT,  // O
        0x13 | SHIFT,  // P
        0x14 | SHIFT,  // Q
        0x15 | SHIFT,  // R
        0x16 | SHIFT,  // S
        0x17 | SHIFT,  // T
        0x18 | SHIFT,  // U
        0x19 | SHIFT,  // V
        0x1a | SHIFT,  // W
        0x1b | SHIFT,  // X
        0x1c | SHIFT,  // Y
        0x1d | SHIFT,  // Z
        0x2f,          // [
        0x31,          // bslash
        0x30,          // ]
        0x23 | SHIFT,  // ^
        0x2d | SHIFT,  // _
        0x35,          // `
        0x04,          // a
        0x05,          // b
        0x06,          // c
        0x07,          // d
        0x08,          // e
        0x09,          // f
        0x0a,          // g
        0x0b,          // h
        0x0c,          // i
        0x0d,          // j
        0x0e,          // k
        0x0f,          // l
        0x10,          // m
        0x11,          // n
        0x12,          // o
        0x13,          // p
        0x14,          // q
        0x15,          // r
        0x16,          // s
        0x17,          // t
        0x18,          // u
        0x19,          // v
        0x1a,          // w
        0x1b,          // x
        0x1c,          // y
        0x1d,          // z
        0x2f | SHIFT,  //
        0x31 | SHIFT,  // |
        0x30 | SHIFT,  // }
        0x35 | SHIFT,  // ~
        0              // DEL
};

typedef struct
{
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
} KeyReport;
static KeyReport _keyReport;

uint8_t transmit_keyboard_key_pressed(uint8_t pKey) {
    uint8_t i;
    if (pKey >= 136) {  // it's a non-printing key (not a modifier)
        pKey = pKey - 136;
    } else if (pKey >= 128) {  // it's a modifier key
        _keyReport.modifiers |= (1 << (pKey - 128));
        pKey = 0;
    } else {  // it's a printing key
        pKey = _asciimap[pKey];
        if (!pKey) {
            return 0;
        }
        if (pKey & 0x80) {                 // it's a capital letter or other character reached with shift
            _keyReport.modifiers |= 0x02;  // the left shift modifier
            pKey &= 0x7F;
        }
    }

    // Add pKey to the key report only if it's not already present
    // and if there is an empty slot.
    if (_keyReport.keys[0] != pKey && _keyReport.keys[1] != pKey &&
        _keyReport.keys[2] != pKey && _keyReport.keys[3] != pKey &&
        _keyReport.keys[4] != pKey && _keyReport.keys[5] != pKey) {
        for (i = 0; i < 6; i++) {
            if (_keyReport.keys[i] == 0x00) {
                _keyReport.keys[i] = pKey;
                break;
            }
        }
        if (i == 6) {
            return 0;
        }
    }

    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)(&_keyReport), sizeof(_keyReport));
    return 1;
}

uint8_t transmit_keyboard_key_released(uint8_t pKey) {
    uint8_t i;
    if (pKey >= 136) {  // it's a non-printing key (not a modifier)
        pKey = pKey - 136;
    } else if (pKey >= 128) {  // it's a modifier key
        _keyReport.modifiers &= ~(1 << (pKey - 128));
        pKey = 0;
    } else {  // it's a printing key
        pKey = _asciimap[pKey];
        if (!pKey) {
            return 0;
        }
        if (pKey & 0x80) {                    // it's a capital letter or other character reached with shift
            _keyReport.modifiers &= ~(0x02);  // the left shift modifier
            pKey &= 0x7F;
        }
    }

    // Test the key report to see if pKey is present.  Clear it if it exists.
    // Check all positions in case the key is present more than once (which it shouldn't be)
    for (i = 0; i < 6; i++) {
        if (0 != pKey && _keyReport.keys[i] == pKey) {
            _keyReport.keys[i] = 0x00;
        }
    }

    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)(&_keyReport), sizeof(_keyReport));
    return 1;
}

#endif  // USBDEVICE_MOUSE_KEYBOARD
#endif  // USE_USBDEVICE
