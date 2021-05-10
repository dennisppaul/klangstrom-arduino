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

#define KLST_CORE           1
#define KLST_TINY           2

/* buttons */

#define KLST_BUTTON_ENCODER_00          0x00
#define KLST_BUTTON_ENCODER_01          0x01
#define KLST_BUTTON_ENCODER_02          0x02
#define KLST_BUTTON_PROGRAMMER          0x03

/* data */

//@todo(maybe move this to `KlangstromDefines`)
#define KLST_SERIAL_00           0x03  
#define KLST_SERIAL_01           0x04  

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

#endif /* KlangstromDefinesArduino_h */
