#include "stm32f4xx_hal.h"
#include "TinyUSB.h"
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;

enum  {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void setup() {
    Klang::lock();

    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL,  mADSR,   Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,   Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_amplitude(0.5);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);

    Klang::unlock();
    tusb_init();
}

void loop() {
    tud_task(); // tinyusb device task
    led_blinking_task();
    midi_task();
    KLST_task(100);
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// MIDI Task
//--------------------------------------------------------------------+

bool mMIDIEventReceived = false;

// Variable that holds the current position in the sequence.
uint32_t note_pos = 0;

// Store example melody as an array of note values
uint8_t note_sequence[] =
{
    74, 78, 81, 86, 90, 93, 98, 102, 57, 61, 66, 69, 73, 78, 81, 85, 88, 92, 97, 100, 97, 92, 88, 85, 81, 78,
    74, 69, 66, 62, 57, 62, 66, 69, 74, 78, 81, 86, 90, 93, 97, 102, 97, 93, 90, 85, 81, 78, 73, 68, 64, 61,
    56, 61, 64, 68, 74, 78, 81, 86, 90, 93, 98, 102
};

void midi_task(void)
{
    static uint32_t start_ms = 0;

    uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
    uint8_t const channel   = 0; // 0 for channel 1

    // The MIDI interface always creates input and output port/jack descriptors
    // regardless of these being used or not. Therefore incoming traffic should be read
    // (possibly just discarded) to avoid the sender blocking in IO
    uint8_t packet[4];
    mMIDIEventReceived = false;
    while ( tud_midi_available() ) {
        tud_midi_packet_read(packet);
        mMIDIEventReceived = true;
    }

    // send note every 1000 ms
    if (millis() - start_ms < 286) return; // not enough time
    start_ms += 286;

    // Previous positions in the note sequence.
    int previous = note_pos - 1;

    // If we currently are at position 0, set the
    // previous position to the last note in the sequence.
    if (previous < 0) previous = sizeof(note_sequence) - 1;

    // Send Note On for current position at full velocity (127) on channel 1.
    uint8_t note_on[3] = { 0x90 | channel, note_sequence[note_pos], 127 };
    tud_midi_stream_write(cable_num, note_on, 3);

    // Send Note Off for previous note.
    uint8_t note_off[3] = { 0x80 | channel, note_sequence[previous], 0};
    tud_midi_stream_write(cable_num, note_off, 3);

    // Increment position
    note_pos++;

    // If we are at the end of the sequence, start over.
    if (note_pos >= sizeof(note_sequence)) note_pos = 0;
}

//--------------------------------------------------------------------+
// KLST TASK
//--------------------------------------------------------------------+
uint8_t mEventCounter = 0;

void KLST_task(uint32_t pInterval) {
    if (mMIDIEventReceived) {
        mEventCounter++;
    }
//#define M_LISTEN_TO_MIDI_EVENTS
#ifdef M_LISTEN_TO_MIDI_EVENTS
    if (mEventCounter == 0) {
        mADSR.start();
        digitalWrite(LED_02, TRUE);
    } else if (mEventCounter == 11) {
        mADSR.stop();
        digitalWrite(LED_02, FALSE);
    }
    mEventCounter %= 24;
#else
    static uint32_t start_ms = 0;
    static bool adsr_state = false;

    if ( millis() - start_ms < pInterval) return;
    start_ms += pInterval;

    if (adsr_state) {
        mADSR.start();
    } else {
        mADSR.stop();
    }
    digitalWrite(LED_02, adsr_state);
    adsr_state = 1 - adsr_state;
#endif
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // Blink every interval ms
    if ( millis() - start_ms < blink_interval_ms) return; // not enough time
    start_ms += blink_interval_ms;

    digitalWrite(LED_01, led_state);
    led_state = 1 - led_state; // toggle
}
