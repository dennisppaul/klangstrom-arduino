#include "stm32f4xx_hal.h"
#include <TinyUSB.h>

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

//--------------------------------------------------------------------+

#define CFG_TUSB_DEBUG_PRINTF printf
#define UART_DEV              USART2
#define UART_GPIO_PORT        GPIOA
#define UART_GPIO_AF          GPIO_AF7_USART2
#define UART_TX_PIN           GPIO_PIN_2
#define UART_RX_PIN           GPIO_PIN_3
#define CFG_BOARD_UART_BAUDRATE 115200
UART_HandleTypeDef UartHandle;

// Default logging with on-board UART
TU_ATTR_USED int _write (int fhdl, const void *buf, size_t count)
{
    (void) fhdl;
    return board_uart_write(buf, count);
}

int board_uart_write(void const * buf, int len) {
#ifdef UART_DEV
    uint8_t data[1];
    for (int i = 0; i < len; i++) {
        data[0] = ((uint8_t*)buf)[i];
        HAL_UART_Transmit(&UartHandle, data, 1, 0xffff);
    }
    return len;
#else
    (void) buf; (void) len; (void) UartHandle;
    return 0;
#endif
}

void initSerialDebug() {
    // UART Enable as the debug log UART
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // UART
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin       = UART_TX_PIN | UART_RX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART_GPIO_AF;
    HAL_GPIO_Init(UART_GPIO_PORT, &GPIO_InitStruct);

    UartHandle.Instance = UART_DEV;
    UartHandle.Init.BaudRate = CFG_BOARD_UART_BAUDRATE;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandle);
}

//--------------------------------------------------------------------+

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

    initSerialDebug();
    tusb_init();
}

void loop() {
    tud_task(); // tinyusb device task
    led_blinking_task();
    midi_task();
    serial_print_task();
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
        print_packet(packet);
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

//--------------------------------------------------------------------+
// SERIAL PRINT TASK
//--------------------------------------------------------------------+
void print_packet(uint8_t pPacket[4]) {
#define UART_BUFFER_LENGTH 1
    int data[UART_BUFFER_LENGTH];
    for (uint8_t i = 0; i < 4; i++) {
        data[0] = pPacket[i] > 0 ? '+' : '-';
        HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    }
    data[0] = '\r';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    data[0] = '\n';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
}

void print_ok() {
#define UART_BUFFER_LENGTH 1
    int data[UART_BUFFER_LENGTH];
    data[0] = 'O';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    data[0] = 'K';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    data[0] = '!';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    data[0] = '\r';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
    data[0] = '\n';
    HAL_UART_Transmit(&UartHandle, (uint8_t*)data, UART_BUFFER_LENGTH, 0xFFFF);
}

void serial_print_task(void) {
    static uint32_t start_ms = 0;
    static uint32_t print_interval_ms = 3333;

    // Blink every interval ms
    if ( millis() - start_ms < print_interval_ms) return; // not enough time
    start_ms += print_interval_ms;

    print_ok();
}
