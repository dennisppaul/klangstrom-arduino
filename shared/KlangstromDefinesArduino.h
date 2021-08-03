//
// KlangstromDefinesArduino.h
// Klang
//
//
//

#ifndef KlangstromDefinesArduino_h
#define KlangstromDefinesArduino_h

/* *premature* klang defines */

typedef float       SIGNAL_TYPE;

/* platform / architecture */

#define KLST_ARCH_MCU       1
#define KLST_ARCH_CPU       2
#define KLST_ARCH_DESKTOP   2
#define KLST_ARCH_VCV       3
#define KLST_ARCH_PLUGIN    3

/* buttons */

#define KLST_BUTTON_ENCODER_00          0x00
#define KLST_BUTTON_ENCODER_01          0x01
#define KLST_BUTTON_ENCODER_02          0x02
#define KLST_BUTTON_PROGRAMMER          0x03

/* options */

// enum KLST_OPTION { AUDIO_INPUT, ENCODERS, SERIAL_PORTS, BEAT}; // @todo(consider replacing defines with enum)

#define KLST_OPTION_AUDIO_INPUT         0x03
#define KLST_MIC                            0x00
#define KLST_LINE_IN                        0x01
#define KLST_OPTION_ENCODERS            0x04
#define KLST_DISABLE                        0x00
#define KLST_ENABLE                         0x01
#define KLST_OPTION_SERIAL_PORTS        0x05
#define KLST_OPTION_BEAT                0x06
#define KLST_OPTION_PROGRAMMER_BUTTON   0x07
#define KLST_OPTION_ENABLE_AUDIO_INPUT  0x08
#define KLST_OPTION_SERIAL_00_BAUD_RATE 0x09
#define KLST_OPTION_SERIAL_01_BAUD_RATE 0x0A
#define KLST_OPTION_SERIAL_02_BAUD_RATE 0x0B

/* UIDs */

#define KLST_NUM_OF_U_ID 10
static const int16_t KLST_NO_ID = -1;
static const uint32_t KLST_U_ID[KLST_NUM_OF_U_ID][3] = {
    {0x003E000A, 0x0200C000, 0x445243EC}, // KLST_TINY_00
    {0x0040001D, 0x0200C000, 0x445243EC},
    {0x00000000, 0x00000000, 0x00000000},
    {0x0040000E, 0x0200C000, 0x445243EC},
    {0x00000000, 0x00000000, 0x00000000},
    {0x003F000D, 0x0200C000, 0x445243EC},
    {0x00000000, 0x00000000, 0x00000000},
    {0x007F6999, 0x02762964, 0x44C7AD50},
    {0x0025003E, 0x0200C000, 0x445243EC},
    {0x0040001C, 0x0200C000, 0x445243EC}, // KLST_TINY_09
};

/* DEBUGGING */

#define DEBUG_DEFINE(x) STR(x)
#define STR(x) #x

#endif /* KlangstromDefinesArduino_h */
