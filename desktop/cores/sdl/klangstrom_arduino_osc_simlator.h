//
//  klangstrom_arduino_osc_simlator.h
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef klangstrom_arduino_osc_simlator_h
#define klangstrom_arduino_osc_simlator_h

#define OSC_TRANSMIT_SIM "/klang/sim"

const static uint8_t    KLST_OSC_SIM_DIGITAL_READ   = 0x83; // @todo(writes into local LED state array ) 
const static uint8_t    KLST_OSC_SIM_DIGITAL_WRITE  = 0x84;
const static uint8_t    KLST_OSC_SIM_LED            = 0x85;
const static uint8_t    KLST_OSC_SIM_BEAT           = 0x86;
const static uint8_t    KLST_OSC_ENCODER_ROTATE     = 0x87;
const static uint8_t    KLST_OSC_ENCODER_BUTTON     = 0x88;

#endif /* klangstrom_arduino_osc_simlator_h */
 