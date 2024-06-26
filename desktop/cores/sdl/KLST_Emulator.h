#ifndef KSLT_emulator_h
#define KSLT_emulator_h

#include <vector>

#include "Arduino.h"
#include "KLST_SDL-adapter.h"
#include "ip/UdpSocket.h"
#include "osc/OscOutboundPacketStream.h"
#include "osc/OscPacketListener.h"
#include "osc/OscReceivedElements.h"

class KLST_Emulator {
    const static uint8_t KLST_OSC_SIM_DIGITAL_READ  = 0xF0;
    const static uint8_t KLST_OSC_SIM_DIGITAL_WRITE = 0xF1;
    const static uint8_t KLST_OSC_SIM_LED           = 0xF2;
    const static uint8_t KLST_OSC_SIM_BEAT          = 0xF3;

public:
    void     led(uint8_t pLED, bool pState);
    void     led_toggle(uint8_t pLED);
    uint32_t analogRead(uint32_t pPin);
    int      digitalRead(uint32_t pPin);
    void     digitalWrite(uint32_t pPin, int8_t pValue);
    void     process_receive(const osc::ReceivedMessage &msg);
    bool    *getLEDs();

private:
    const static uint8_t NUMBER_OF_PINS        = NUMBER_OF_GPIO_PINS;
    int8_t               mPins[NUMBER_OF_PINS] = {LOW};
    bool                 mLEDs[NUMBER_OF_LEDS] = {false};

    bool str_begins_with(const char *pre, const char *str);
    bool str_equals(const char *pStringA, const char *pStringB);
    void emit_message_error(const char *pMessage, const char *pTypeTag, const osc::ReceivedMessage &msg);
};
#endif /* KSLT_emulator_h */
