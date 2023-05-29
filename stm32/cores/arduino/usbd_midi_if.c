/**
 ******************************************************************************
 * @file    usbd_midi_if.c
 * @author  Sam Kent
 * @brief   USB MIDI Access Layer
 ******************************************************************************
 */

#if defined(USBDEVICE_MIDI)

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi_if.h"

#include "USBDevice.h"

static const uint8_t MIDI_NONE                  = 0x00;
static const uint8_t MIDI_NOTE_OFF              = 0x80;
static const uint8_t MIDI_NOTE_ON               = 0x90;
static const uint8_t MIDI_AFTERTOUCH            = 0xA0;
static const uint8_t MIDI_CONTROL_CHANGE        = 0xB0;
static const uint8_t MIDI_PROGRAM_CHANGE        = 0xC0;
static const uint8_t MIDI_CHANNEL_PRESSURE      = 0xD0;
static const uint8_t MIDI_PITCHBEND             = 0xE0;
static const uint8_t MIDI_SYS_EX                = 0xF0;
static const uint8_t MIDI_MTCQFRAME             = 0xF1;  // dataB = quarter frame number
static const uint8_t MIDI_SONG_POSITION_POINTER = 0xF2;
static const uint8_t MIDI_SONG_SELECT           = 0xF3;  // dataB = song#
static const uint8_t MIDI_UNDEF_1               = 0xF4;
static const uint8_t MIDI_UNDEF_2               = 0xF5;
static const uint8_t MIDI_TUNE_REQ              = 0xF6;  // no data, return oscillators
static const uint8_t MIDI_ENDEX                 = 0xF7;  // ends a SYSEX dump
static const uint8_t MIDI_CLOCK_TICK            = 0xF8;
static const uint8_t MIDI_RT_CLOCK              = 0xF8;  // *
static const uint8_t MIDI_RT_UNDEF_1            = 0xF9;
static const uint8_t MIDI_CLOCK_START           = 0xFA;
static const uint8_t MIDI_RT_START              = 0xFA;  // *
static const uint8_t MIDI_CLOCK_CONTINUE        = 0xFB;
static const uint8_t MIDI_RT_CONTINUE           = 0xFB;  // *
static const uint8_t MIDI_CLOCK_STOP            = 0xFC;
static const uint8_t MIDI_RT_STOP               = 0xFC;  // *
static const uint8_t MIDI_RT_UNDEF_2            = 0xFD;
static const uint8_t MIDI_RT_SENSE              = 0xFE;  // Sent every 300ms on a live connection. Can ignore.
static const uint8_t MIDI_RT_RESET              = 0xFF;  // Reset any parameters to their power up values

/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USBD_MIDI
 * @brief usbd core module
 * @{
 */

/** @defgroup USBD_MIDI_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_MIDI_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_MIDI_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_MIDI_Private_FunctionPrototypes
 * @{
 */

int8_t MIDI_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx);
int8_t MIDI_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx);
int8_t MIDI_Receive(uint8_t* pbuf, uint32_t length);
int8_t MIDI_Send(uint8_t* pbuf, uint32_t length);

USBD_MIDI_ItfTypeDef USBD_MIDI_fops = {MIDI_Init,
                                       MIDI_DeInit,
                                       MIDI_Receive,
                                       MIDI_Send};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  MIDI_Init
 *         Initializes the MIDI media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t MIDI_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx) {
    /*
       Add your initialization code here
    */
    return (0);
}

/**
 * @brief  MIDI_DeInit
 *         DeInitializes the MIDI media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t MIDI_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx) {
    /*
       Add your deinitialization code here
    */
    return (0);
}

/**
 * @brief  MIDI_Send
 *
 * @param  buffer: bufferfer of data to be received
 * @param  length: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t MIDI_Send(uint8_t* buffer, uint32_t length) {
    uint8_t ret = USBD_OK;
    USBD_MIDI_SetTxBuffer(&hUsbDeviceFS, buffer, length);
    ret = USBD_MIDI_TransmitPacket(&hUsbDeviceFS);
    return (ret);
}

static void parseSysExMessage(uint8_t* message) {
    const uint8_t sys_ex_message_type = message[0] & 0xFF;  // // bits 7-3 of the status byte
    uint8_t       lsb, msb, song_number;
    uint16_t      song_position;

    switch (sys_ex_message_type) {
        case MIDI_SYS_EX:  // System Exclusive
                           // The second byte is the first data byte of the system exclusive message
                           // Do something with the system exclusive message
            usb_device_call_midi_sys_ex(message);
            break;
        case MIDI_MTCQFRAME:  // MIDI Time Code Quarter Frame
                              // The second byte contains the time code data
                              // Do something with the MIDI Time Code Quarter Frame message
            usb_device_call_midi_mtc_quarter_frame(message[1]);
            break;
        case MIDI_SONG_POSITION_POINTER:
            // The second byte is the least significant 7 bits of the song position, and the third byte
            // is the most significant 7 bits
            lsb           = message[1];
            msb           = message[2];
            song_position = (msb << 7) | lsb;
            usb_device_call_midi_song_position_pointer(song_position);
            break;
        case MIDI_SONG_SELECT:  // Song Select
                                // The second byte is the song number
            song_number = message[1];
            usb_device_call_midi_song_select(song_number);
            break;
        case MIDI_TUNE_REQ:  // Tune Request
            usb_device_call_midi_tune_req();
            break;
        case MIDI_ENDEX:  // End of System Exclusive
            usb_device_call_midi_end_ex();
            break;
        case MIDI_CLOCK_TICK:
            usb_device_call_midi_clock_tick();
            break;
        case MIDI_CLOCK_START:
            usb_device_call_midi_clock_start();
            break;
        case MIDI_CLOCK_CONTINUE:
            usb_device_call_midi_clock_continue();
            break;
        case MIDI_CLOCK_STOP:
            usb_device_call_midi_clock_stop();
            break;
        case MIDI_RT_SENSE:
            usb_device_call_midi_rt_sense();
            break;
        case MIDI_RT_RESET:
            usb_device_call_midi_rt_reset();
            break;
    }
}

static void parseMIDIMessage(uint8_t* message) {
    const uint8_t status_byte  = message[0];
    const uint8_t message_type = status_byte & 0xF0;  // bits 7-4 of the status byte
    const uint8_t channel      = status_byte & 0x0F;  // bits 3-0 of the status byte
    uint8_t       note, velocity, pressure, lsb, msb, controller, value, program;
    uint16_t      pitch_bend;

    switch (message_type) {
        case MIDI_NOTE_OFF:
            note     = message[1];
            velocity = message[2];
            usb_device_call_midi_note_off(channel, note, velocity);
            break;
        case MIDI_NOTE_ON:
            note     = message[1];
            velocity = message[2];
            usb_device_call_midi_note_on(channel, note, velocity);
            break;
        case MIDI_AFTERTOUCH:
            note     = message[1];
            pressure = message[2];
            usb_device_call_midi_aftertouch(channel, note, pressure);
            break;
        case MIDI_CONTROL_CHANGE:
            controller = message[1];
            value      = message[2];
            usb_device_call_midi_control_change(channel, controller, value);
            break;
        case MIDI_PROGRAM_CHANGE:
            program = message[1];
            usb_device_call_midi_program_change(channel, program);
            break;
        case MIDI_CHANNEL_PRESSURE:
            pressure = message[1];
            usb_device_call_midi_channel_pressure(channel, pressure);
            break;
        case MIDI_PITCHBEND:
            // The second byte of the message is the least significant 7 bits of the pitch bend value, and the third byte is the most significant 7 bits
            lsb        = message[1];
            msb        = message[2];
            pitch_bend = (msb << 7) | lsb;
            usb_device_call_midi_pitch_bend(channel, pitch_bend);
            break;
        case MIDI_SYS_EX:
            parseSysExMessage(message);
            break;
    }
}

/* this TMP hack is required as long as we need to copy files to the core folder :( )*/
typedef struct {
    uint8_t  channel;
    uint8_t  command;
    uint16_t a;
    uint8_t  b;
} TMP___EventMIDI;
static const uint8_t TMP___EVENT_MIDI = 0x0B;

/**
 * @brief  MIDI_Receive
 *
 * @param  buffer: buffer of data to be received
 * @param  length: number of bytes received
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t MIDI_Receive(uint8_t* buffer, uint32_t length) {
    /* this TMP hack is required as long as we need to copy files to the core folder :( )*/
    TMP___EventMIDI mEvent;
    mEvent.command = buffer[1] & 0xF0;
    mEvent.channel = buffer[1] & 0x0F;
    mEvent.a       = buffer[2];
    mEvent.b       = buffer[3];
    usb_device_call_event_receive(TMP___EVENT_MIDI, &mEvent);

    uint8_t mMessage[3] = {buffer[1], buffer[2], buffer[3]};
    parseMIDIMessage(mMessage);
    return 0;
}

void MIDI_note_on(uint8_t note, uint8_t velocity) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0x90;
    b[2] = note;
    b[3] = velocity;

    MIDI_Send(b, 4);
}

void MIDI_note_off(uint8_t note, uint8_t velocity) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0x80;
    b[2] = note;
    b[3] = velocity;

    MIDI_Send(b, 4);
}

void MIDI_cc(uint8_t channel, uint8_t number, uint8_t value) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0xB0 | channel;
    b[2] = number;
    b[3] = value;

    MIDI_Send(b, 4);
}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif  // USBDEVICE_MIDI

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
