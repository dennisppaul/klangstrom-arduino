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

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

WEAK void receive_midi_note_off(const uint8_t channel, const uint8_t note) {}
WEAK void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {}
WEAK void receive_midi_control_change(const uint8_t channel, const uint8_t number, const uint8_t value) {}

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

/**
 * @brief  MIDI_Receive
 *
 * @param  buffer: buffer of data to be received
 * @param  length: number of bytes received
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t MIDI_Receive(uint8_t* buffer, uint32_t length) {
    uint8_t  channel = buffer[1] & 0x0F;
    uint8_t  msgtype = buffer[1] & 0xF0;
    uint8_t  b1      = buffer[2];
    uint8_t  b2      = buffer[3];
    uint16_t b       = ((b2 & 0x7f) << 7) | (b1 & 0x7f);

    USBD_UsrLog("MIDI receive channel:0x%02X msg:0x%02X b1:0x%02X b2:0x%02X", channel, msgtype, b1, b2);

    static const uint8_t NOTE_OFF                = 0x80;
    static const uint8_t NOTE_ON                 = 0x90;
    static const uint8_t CONTROL_CHANGE          = 0xB0;
    static const uint8_t PROGRAM_CHANGE          = 0xC0;
    static const uint8_t SYSTEM_REALTIME_MESSAGE = 0xF0;

    switch (msgtype) {
        case NOTE_OFF:
            USBD_UsrLog("NOTE_OFF");
            receive_midi_note_off(channel, b1);
            break;
        case NOTE_ON:
            USBD_UsrLog("NOTE_ON");
            receive_midi_note_on(channel, b1, b2);
            break;
        case CONTROL_CHANGE:
            USBD_UsrLog("CONTROL_CHANGE");
            receive_midi_control_change(channel, b1, b2);
            break;
        case PROGRAM_CHANGE:
            USBD_UsrLog("PROGRAM_CHANGE");
            // receive_midi_program_change(channel, b1, b2);
            break;
        case SYSTEM_REALTIME_MESSAGE:
            USBD_UsrLog("SYSTEM_REALTIME_MESSAGE");
            break;
        default:
            break;
    }
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
