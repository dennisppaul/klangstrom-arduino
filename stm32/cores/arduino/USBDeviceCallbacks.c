#ifdef USE_USBDEVICE

#include "USBDevice.h"

static CallbackType2_UI8_VOIDPTR pCallbackEventReceived = NULL;
void                             usb_device_callback_event_receive(CallbackType2_UI8_VOIDPTR callback) {
    pCallbackEventReceived = callback;
}

void usb_device_call_event_receive(const uint8_t event, const void *data) {
    if (pCallbackEventReceived) {
        pCallbackEventReceived(event, data);
    }
}

/* --- MIDI --- */

static CallbackType3_UI8 pCallbackMidiNoteOff = NULL;
void                     usb_device_callback_midi_note_off(CallbackType3_UI8 callback) {
    pCallbackMidiNoteOff = callback;
}

void usb_device_call_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    if (pCallbackMidiNoteOff) {
        pCallbackMidiNoteOff(channel, note, velocity);
    }
}

static CallbackType3_UI8 pCallbackMidiNoteOn = NULL;
void                     usb_device_callback_midi_note_on(CallbackType3_UI8 callback) {
    pCallbackMidiNoteOn = callback;
}

void usb_device_call_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    if (pCallbackMidiNoteOn) {
        pCallbackMidiNoteOn(channel, note, velocity);
    }
}

// void usb_device_callback_midi_aftertouch(CallbackType3_UI8 callback);
static CallbackType3_UI8 p_callback_midi_aftertouch = NULL;
void                     usb_device_callback_midi_aftertouch(CallbackType3_UI8 callback) {
    p_callback_midi_aftertouch = callback;
}

void usb_device_call_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure) {
    if (p_callback_midi_aftertouch) {
        p_callback_midi_aftertouch(channel, note, pressure);
    }
}

// void usb_device_callback_midi_control_change(CallbackType3_UI8 callback);
static CallbackType3_UI8 p_callback_midi_control_change = NULL;
void                     usb_device_callback_midi_control_change(CallbackType3_UI8 callback) {
    p_callback_midi_control_change = callback;
}

void usb_device_call_midi_control_change(const uint8_t channel, const uint8_t control, const uint8_t value) {
    if (p_callback_midi_control_change) {
        p_callback_midi_control_change(channel, control, value);
    }
}

// void usb_device_callback_midi_program_change(CallbackType2_UI8 callback);
static CallbackType2_UI8 p_callback_midi_program_change = NULL;
void                     usb_device_callback_midi_program_change(CallbackType2_UI8 callback) {
    p_callback_midi_program_change = callback;
}

void usb_device_call_midi_program_change(const uint8_t channel, const uint8_t program) {
    if (p_callback_midi_program_change) {
        p_callback_midi_program_change(channel, program);
    }
}

// void usb_device_callback_midi_channel_pressure(CallbackType2_UI8 callback);
static CallbackType2_UI8 p_callback_midi_channel_pressure = NULL;
void                     usb_device_callback_midi_channel_pressure(CallbackType2_UI8 callback) {
    p_callback_midi_channel_pressure = callback;
}

void usb_device_call_midi_channel_pressure(const uint8_t channel, const uint8_t pressure) {
    if (p_callback_midi_channel_pressure) {
        p_callback_midi_channel_pressure(channel, pressure);
    }
}

// void usb_device_callback_midi_pitch_bend(CallbackType2_UI8_I16 callback);
static CallbackType2_UI8_I16 p_callback_midi_pitch_bend = NULL;
void                         usb_device_callback_midi_pitch_bend(CallbackType2_UI8_I16 callback) {
    p_callback_midi_pitch_bend = callback;
}

void usb_device_call_midi_pitch_bend(const uint8_t channel, const uint16_t pitch) {
    if (p_callback_midi_pitch_bend) {
        p_callback_midi_pitch_bend(channel, pitch);
    }
}

// void usb_device_callback_midi_sys_ex(CallbackType0 callback);
static CallbackType0 p_callback_midi_sys_ex = NULL;
void                 usb_device_callback_midi_sys_ex(CallbackType0 callback) {
    p_callback_midi_sys_ex = callback;
}

void usb_device_call_midi_sys_ex() {
    if (p_callback_midi_sys_ex) {
        p_callback_midi_sys_ex();
    }
}

// void usb_device_callback_midi_mtc_quarter_frame(CallbackType1_UI8 callback) { callback_mtc_quarter_frame = callback; }
static CallbackType1_UI8 p_callback_midi_mtc_quarter_frame = NULL;
void                     usb_device_callback_midi_mtc_quarter_frame(CallbackType1_UI8 callback) {
    p_callback_midi_mtc_quarter_frame = callback;
}

void usb_device_call_midi_mtc_quarter_frame(uint8_t frame_number) {
    if (p_callback_midi_mtc_quarter_frame) {
        p_callback_midi_mtc_quarter_frame(frame_number);
    }
}

// void usb_device_callback_midi_song_position_pointer(CallbackType1_UI16 callback) { callback_song_position_pointer = callback; }
static CallbackType1_UI16 p_callback_midi_song_position_pointer = NULL;
void                      usb_device_callback_midi_song_position_pointer(CallbackType1_UI16 callback) {
    p_callback_midi_song_position_pointer = callback;
}

void usb_device_call_midi_song_position_pointer(uint16_t song_position) {
    if (p_callback_midi_song_position_pointer) {
        p_callback_midi_song_position_pointer(song_position);
    }
}

// void usb_device_callback_midi_song_select(CallbackType1_UI8 callback) { callback_song_select = callback; }
static CallbackType1_UI8 p_callback_midi_song_select = NULL;
void                     usb_device_callback_midi_song_select(CallbackType1_UI8 callback) {
    p_callback_midi_song_select = callback;
}

void usb_device_call_midi_song_select(uint8_t song_number) {
    if (p_callback_midi_song_select) {
        p_callback_midi_song_select(song_number);
    }
}

// void usb_device_callback_midi_tune_req(CallbackType0 callback) { callback_tune_req = callback; }
static CallbackType0 p_callback_midi_tune_req = NULL;
void                 usb_device_callback_midi_tune_req(CallbackType0 callback) {
    p_callback_midi_tune_req = callback;
}

void usb_device_call_midi_tune_req() {
    if (p_callback_midi_tune_req) {
        p_callback_midi_tune_req();
    }
}

// void usb_device_callback_midi_end_ex(CallbackType0 callback) { callback_end_ex = callback; }
static CallbackType0 p_callback_midi_end_ex = NULL;
void                 usb_device_callback_midi_end_ex(CallbackType0 callback) {
    p_callback_midi_end_ex = callback;
}

void usb_device_call_midi_end_ex() {
    if (p_callback_midi_end_ex) {
        p_callback_midi_end_ex();
    }
}

static CallbackType0 pCallbackMidiClockTick = NULL;
void                 usb_device_callback_midi_clock_tick(CallbackType0 callback) {
    pCallbackMidiClockTick = callback;
}

void usb_device_call_midi_clock_tick() {
    if (pCallbackMidiClockTick) {
        pCallbackMidiClockTick();
    }
}

static CallbackType0 pCallbackMidiClockStart = NULL;
void                 usb_device_callback_midi_clock_start(CallbackType0 callback) {
    pCallbackMidiClockStart = callback;
}

void usb_device_call_midi_clock_start() {
    if (pCallbackMidiClockStart) {
        pCallbackMidiClockStart();
    }
}

static CallbackType0 pCallbackMidiClockContinue = NULL;
void                 usb_device_callback_midi_clock_continue(CallbackType0 callback) {
    pCallbackMidiClockContinue = callback;
}

void usb_device_call_midi_clock_continue() {
    if (pCallbackMidiClockContinue) {
        pCallbackMidiClockContinue();
    }
}

static CallbackType0 pCallbackMidiClockStop = NULL;
void                 usb_device_callback_midi_clock_stop(CallbackType0 callback) {
    pCallbackMidiClockStop = callback;
}

void usb_device_call_midi_clock_stop() {
    if (pCallbackMidiClockStop) {
        pCallbackMidiClockStop();
    }
}

// void usb_device_callback_midi_rt_sense(CallbackType0 callback) { callback_rt_sense = callback; }
static CallbackType0 p_callback_midi_rt_sense = NULL;
void                 usb_device_callback_midi_rt_sense(CallbackType0 callback) {
    p_callback_midi_rt_sense = callback;
}

void usb_device_call_midi_rt_sense() {
    if (p_callback_midi_rt_sense) {
        p_callback_midi_rt_sense();
    }
}

// void usb_device_callback_midi_rt_reset(CallbackType0 callback) { callback_rt_reset = callback; }
static CallbackType0 p_callback_midi_rt_reset = NULL;
void                 usb_device_callback_midi_rt_reset(CallbackType0 callback) {
    p_callback_midi_rt_reset = callback;
}

void usb_device_call_midi_rt_reset() {
    if (p_callback_midi_rt_reset) {
        p_callback_midi_rt_reset();
    }
}

#endif  // USE_USBDEVICE