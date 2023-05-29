#ifdef USE_USBHOST

/* Includes ------------------------------------------------------------------*/

#include "MIDI_application.h"

#include "USBHost.h"
#include "usb_host.h"

/* Private define ------------------------------------------------------------*/

#define RX_BUFF_SIZE 64                                  /* USB MIDI buffer : max received data 64 bytes */
uint8_t                   MIDI_RX_Buffer[RX_BUFF_SIZE];  // MIDI reception buffer
extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHost;
extern bool               usb_host_midi_connected;

/* Private function prototypes -----------------------------------------------*/
void ProcessReceivedMidiDatas(void);

/*-----------------------------------------------------------------------------*/
/**
 * @brief  Main routine for MIDI application, looped in main.c
 * @param  None
 * @retval none
 */
void MIDI_Application(void) {
    if (Appli_state == APPLICATION_READY) {
        USBH_MIDI_Receive(&hUsbHost, MIDI_RX_Buffer, RX_BUFF_SIZE);  // just once at the beginning, start the first reception
        Appli_state = APPLICATION_RUNNING;
    } else if (Appli_state == APPLICATION_RUNNING) {
    } else if (Appli_state == APPLICATION_DISCONNECT) {
        Appli_state = APPLICATION_IDLE;
        USBH_MIDI_Stop(&hUsbHost);
    }
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback.
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost) {
    ProcessReceivedMidiDatas();
    USBH_MIDI_Receive(&hUsbHost, MIDI_RX_Buffer, RX_BUFF_SIZE);  // start a new reception
}

void USBH_MIDI_TransmitCallback(USBH_HandleTypeDef *phost) {
}

static void parseSysExMessage(uint8_t *message) {
    const uint8_t sys_ex_message_type = message[0] & 0xFF;  // // bits 7-3 of the status byte
    uint8_t       lsb, msb, song_number;
    uint16_t      song_position;

    switch (sys_ex_message_type) {
        case 0xF0:  // System Exclusive
                    // The second byte is the first data byte of the system exclusive message
                    // Do something with the system exclusive message
            USBH_DbgLog("System Exclusive");
            usb_host_call_midi_sys_ex(message);
            break;
        case 0xF1:  // MIDI Time Code Quarter Frame
                    // The second byte contains the time code data
                    // Do something with the MIDI Time Code Quarter Frame message
            USBH_DbgLog("MIDI Time Code Quarter Frame");
            usb_host_call_midi_mtc_quarter_frame(message[1]);
            break;
        case MIDI_SONG_POSITION_POINTER:
            // The second byte is the least significant 7 bits of the song position, and the third byte
            // is the most significant 7 bits
            lsb           = message[1];
            msb           = message[2];
            song_position = (msb << 7) | lsb;
            USBH_DbgLog("MIDI_SONG_POSITION_POINTER");
            usb_host_call_midi_song_position_pointer(song_position);
            break;
        case 0xF3:  // Song Select
                    // The second byte is the song number
            song_number = message[1];
            USBH_DbgLog("Song Select");
            usb_host_call_midi_song_select(song_number);
            break;
        case 0xF6:  // Tune Request
            USBH_DbgLog("Tune Request");
            usb_host_call_midi_tune_req();
            break;
        case 0xF7:  // End of System Exclusive
            USBH_DbgLog("End of System Exclusive");
            usb_host_call_midi_end_ex();
            break;
        case MIDI_CLOCK_TICK:
            USBH_DbgLog("MIDI_CLOCK_TICK");
            usb_host_call_midi_clock_tick();
            break;
        case MIDI_CLOCK_START:
            USBH_DbgLog("MIDI_CLOCK_START");
            usb_host_call_midi_clock_start();
            break;
        case MIDI_CLOCK_CONTINUE:
            USBH_DbgLog("MIDI_CLOCK_CONTINUE");
            usb_host_call_midi_clock_continue();
            break;
        case MIDI_CLOCK_STOP:
            USBH_DbgLog("MIDI_CLOCK_STOP");
            usb_host_call_midi_clock_stop();
            break;
        case 0xFE:
            USBH_DbgLog("MIDI_ACTIVE_SENSING");
            usb_host_call_midi_rt_sense();
            break;
        case 0xFF:
            USBH_DbgLog("MIDI_SYSTEM_RESET");
            usb_host_call_midi_rt_reset();
            break;
    }
}

static void parseMIDIMessage(uint8_t *message) {
    const uint8_t status_byte  = message[0];
    const uint8_t message_type = status_byte & 0xF0;  // bits 7-4 of the status byte
    const uint8_t channel      = status_byte & 0x0F;  // bits 3-0 of the status byte
    uint8_t       note, velocity, pressure, lsb, msb, controller, value, program;
    uint16_t      pitch_bend;

    switch (message_type) {
        case MIDI_NOTE_OFF:
            note     = message[1];
            velocity = message[2];
            usb_host_call_midi_note_off(channel, note, velocity);
            USBH_DbgLog("MIDI_NOTE_OFF");
            break;
        case MIDI_NOTE_ON:
            note     = message[1];
            velocity = message[2];
            // if (velocity == 0) {
            //     usb_host_call_midi_note_off(channel, note, velocity);
            //     USBH_DbgLog("MIDI_NOTE_OFF");
            // } else {
            usb_host_call_midi_note_on(channel, note, velocity);
            USBH_DbgLog("MIDI_NOTE_ON");
            // }
            break;
        case MIDI_AFTERTOUCH:
            note     = message[1];
            pressure = message[2];
            USBH_DbgLog("MIDI_AFTERTOUCH");
            usb_host_call_midi_aftertouch(channel, note, pressure);
            break;
        case MIDI_CONTROL_CHANGE:
            controller = message[1];
            value      = message[2];
            USBH_DbgLog("MIDI_CONTROL_CHANGE");
            usb_host_call_midi_control_change(channel, controller, value);
            break;
        case MIDI_PROGRAM_CHANGE:
            program = message[1];
            USBH_DbgLog("MIDI_PROGRAM_CHANGE");
            usb_host_call_midi_program_change(channel, program);
            break;
        case MIDI_CHANNEL_PRESSURE:
            pressure = message[1];
            USBH_DbgLog("MIDI_CHANNEL_PRESSURE");
            usb_host_call_midi_channel_pressure(channel, pressure);
            break;
        case MIDI_PITCHBEND:
            // The second byte of the message is the least significant 7 bits of the pitch bend value, and the third byte is the most significant 7 bits
            lsb        = message[1];
            msb        = message[2];
            pitch_bend = (msb << 7) | lsb;
            USBH_DbgLog("MIDI_PITCHBEND");
            usb_host_call_midi_pitch_bend(channel, pitch_bend);
            break;
        case MIDI_SYS_EX:
            parseSysExMessage(message);
            break;
    }
}

void ProcessReceivedMidiDatas(void) {
    uint16_t       numberOfPackets;
    uint8_t       *ptr = MIDI_RX_Buffer;
    midi_package_t pack;

    numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHost) / 4;  // each USB midi package is 4 bytes long

    while (numberOfPackets--) {
        pack.cin_cable = *ptr;
        ptr++;
        pack.evnt0 = *ptr;
        ptr++;
        pack.evnt1 = *ptr;
        ptr++;
        pack.evnt2 = *ptr;
        ptr++;

        // const uint8_t cable   = pack.cin_cable;
        // const uint8_t channel = pack.evnt0 & 0x0F;
        // // const uint8_t command = pack.evnt0 & 0xF0;
        // const uint8_t command = pack.evnt0 & 0xF8;
        // const uint8_t a       = pack.evnt1;
        // const uint8_t b       = pack.evnt2;

        EventMIDI mEvent;
        mEvent.command = pack.evnt0 & 0xF0;
        mEvent.channel = pack.evnt0 & 0x0F;
        mEvent.a       = pack.evnt1;
        mEvent.b       = pack.evnt2;
        usb_host_call_event_receive(EVENT_MIDI, &mEvent);

        uint8_t mMessage[3] = {pack.evnt0, pack.evnt1, pack.evnt2};
        parseMIDIMessage(mMessage);
    }
}

/* --- transmit MIDI messages --- */

static uint8_t fUSBHostCable_ID = 0;

void usb_host_set_cable_ID(const uint8_t cable) {
    fUSBHostCable_ID = cable;
}

static const uint8_t MIDI_TYPE_InvalidType          = 0x00;  // For notifying errors
static const uint8_t MIDI_TYPE_NoteOff              = 0x80;  // Note Off
static const uint8_t MIDI_TYPE_NoteOn               = 0x90;  // Note On
static const uint8_t MIDI_TYPE_AfterTouchPoly       = 0xA0;  // Polyphonic AfterTouch
static const uint8_t MIDI_TYPE_ControlChange        = 0xB0;  // Control Change / Channel Mode
static const uint8_t MIDI_TYPE_ProgramChange        = 0xC0;  // Program Change
static const uint8_t MIDI_TYPE_AfterTouchChannel    = 0xD0;  // Channel (monophonic) AfterTouch
static const uint8_t MIDI_TYPE_PitchBend            = 0xE0;  // Pitch Bend
static const uint8_t MIDI_TYPE_SystemExclusive      = 0xF0;  // System Exclusive
static const uint8_t MIDI_TYPE_TimeCodeQuarterFrame = 0xF1;  // System Common - MIDI Time Code
static const uint8_t MIDI_TYPE_SystemExclusiveEnd   = 0xF7;  // ends a SYSEX dump Quarter Frame
static const uint8_t MIDI_TYPE_SongPosition         = 0xF2;  // System Common - Song Position Pointer
static const uint8_t MIDI_TYPE_SongSelect           = 0xF3;  // System Common - Song Select
static const uint8_t MIDI_TYPE_TuneRequest          = 0xF6;  // System Common - Tune Request
static const uint8_t MIDI_TYPE_Clock                = 0xF8;  // System Real Time - Timing Clock
static const uint8_t MIDI_TYPE_Start                = 0xFA;  // System Real Time - Start
static const uint8_t MIDI_TYPE_Continue             = 0xFB;  // System Real Time - Continue
static const uint8_t MIDI_TYPE_Stop                 = 0xFC;  // System Real Time - Stop
static const uint8_t MIDI_TYPE_ActiveSensing        = 0xFE;  // System Real Time - Active Sensing
static const uint8_t MIDI_TYPE_SystemReset          = 0xFF;  // System Real Time - System Reset

static uint8_t type2cin[][2] = {{MIDI_TYPE_InvalidType, 0},
                                {MIDI_TYPE_NoteOff, 8},
                                {MIDI_TYPE_NoteOn, 9},
                                {MIDI_TYPE_AfterTouchPoly, 0xA},
                                {MIDI_TYPE_ControlChange, 0xB},
                                {MIDI_TYPE_ProgramChange, 0xC},
                                {MIDI_TYPE_AfterTouchChannel, 0xD},
                                {MIDI_TYPE_PitchBend, 0xE}};

static uint8_t system2cin[][2] = {{MIDI_TYPE_SystemExclusive, 0},
                                  {MIDI_TYPE_TimeCodeQuarterFrame, 2},
                                  {MIDI_TYPE_SongPosition, 3},
                                  {MIDI_TYPE_SongSelect, 2},
                                  {0, 0},
                                  {0, 0},
                                  {MIDI_TYPE_TuneRequest, 5},
                                  {MIDI_TYPE_SystemExclusiveEnd, 0},
                                  {MIDI_TYPE_Clock, 0xF},
                                  {0, 0},
                                  {MIDI_TYPE_Start, 0xF},
                                  {MIDI_TYPE_Continue, 0xF},
                                  {MIDI_TYPE_Stop, 0xF},
                                  {0, 0},
                                  {MIDI_TYPE_ActiveSensing, 0xF},
                                  {MIDI_TYPE_SystemReset, 0xF}};

// static byte cin2Len[][2] = {{0, 0}, {1, 0}, {2, 2}, {3, 3}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 3}, {9, 3}, {10, 3}, {11, 3}, {12, 2}, {13, 2}, {14, 3}, {15, 1}};

#define MAKEHEADER(cn, cin) (((cn & 0x0f) << 4) | cin)

static void usb_host_transmit_usb_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3) {
    // NOTE compute header byte from message type and Cable ID
    // ( see https://www.usb.org/sites/default/files/midi10.pdf )

    uint8_t cin = 0;
    if (data1 < MIDI_TYPE_SystemExclusive) {
        // Non System messages
        cin = type2cin[((data1 & 0xF0) >> 4) - 7][1];
    } else {
        // Only System messages
        cin = system2cin[data1 & 0x0F][1];
    }
    uint8_t mPacketHeader = MAKEHEADER(fUSBHostCable_ID, cin);

    if (usb_host_midi_connected) {
        const uint8_t length = 4;
        uint8_t       data[length];
        data[0] = mPacketHeader;
        data[1] = data1;
        data[2] = data2;
        data[3] = data3;
        data[0] = 0xFB;
        data[1] = 0xFB;
        data[2] = 0xFB;
        data[3] = 0xFB;
        USBH_MIDI_Transmit(&hUsbHost, data, length);
    }
}

void usb_host_transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3) {
    usb_host_transmit_usb_midi_message(data1, data2, data3);
}

void usb_host_transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    const uint8_t mCMD = MIDI_NOTE_OFF | channel;
    usb_host_transmit_midi_message(mCMD, note, velocity);
}

void usb_host_transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    const uint8_t mCMD = MIDI_NOTE_ON | channel;
    usb_host_transmit_midi_message(mCMD, note, velocity);
}

void usb_host_transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure) {
    const uint8_t mCMD = MIDI_AFTERTOUCH | channel;
    usb_host_transmit_midi_message(mCMD, note, pressure);
}

void usb_host_transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value) {
    const uint8_t mCMD = MIDI_CONTROL_CHANGE | channel;
    usb_host_transmit_midi_message(mCMD, command, value);
}

void usb_host_transmit_midi_program_change(const uint8_t channel, const uint8_t program) {
    const uint8_t mCMD = MIDI_PROGRAM_CHANGE | channel;
    usb_host_transmit_midi_message(mCMD, program, 0);
}

void usb_host_transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure) {
    const uint8_t mCMD = MIDI_CHANNEL_PRESSURE | channel;
    usb_host_transmit_midi_message(mCMD, pressure, 0);
}

void usb_host_transmit_midi_pitch_bend(const uint8_t channel, const int16_t value) {
    // @todo(test this!)
    const uint16_t mValue   = (uint16_t)value;
    const uint16_t LSB_MASK = 0b00000001111111;
    const uint16_t MSB_MASK = 0b11111110000000;
    const uint8_t  msb      = (mValue & MSB_MASK) / 128;
    const uint8_t  lsb      = mValue & LSB_MASK;
    const uint8_t  mCMD     = MIDI_PITCHBEND | channel;
    usb_host_transmit_midi_message(mCMD, lsb, msb);
}

void usb_host_transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
    usb_host_transmit_midi_message(type, data1, data2);
}

/*-----------------------------------------------------------------------------*/

#endif  // USE_USBHOST
