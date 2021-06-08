//
//  KLST_Simulator.h
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef KSLT_simulator_h
#define KSLT_simulator_h

#include <vector>
#include "klangstrom_arduino_sdl.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "KlangstromApplicationArduino.h"

using namespace klangstrom;

void klangstrom_arduino_sim_transmit(std::vector<float> &pData);
void event_receive(const EVENT_TYPE event, const float* data);
void data_receive(const uint8_t sender, uint8_t* data, uint8_t length);

class KLST_Simulator {

    const static uint8_t    KLST_OSC_SIM_DIGITAL_READ   = 0xF0; 
    const static uint8_t    KLST_OSC_SIM_DIGITAL_WRITE  = 0xF1;
    const static uint8_t    KLST_OSC_SIM_LED            = 0xF2;
    const static uint8_t    KLST_OSC_SIM_BEAT           = 0xF3;

public:
    void led(uint8_t pLED, bool pState) {
        const uint8_t mLED = pLED - LED_00; 
        const uint8_t mID = (mLED >= NUMBER_OF_LEDS) ? (NUMBER_OF_LEDS-1) : mLED;
        mLEDs[mID] = pState;

        std::vector<float> mData;
        mData.push_back(KLST_OSC_SIM_LED);
        mData.push_back(0);
        mData.push_back(0);
        klangstrom_arduino_sim_transmit(mData);
    }

    void led_toggle(uint8_t pLED) {
        const uint8_t mLED = pLED - LED_00; 
        const uint8_t mID = (mLED >= NUMBER_OF_LEDS) ? (NUMBER_OF_LEDS-1) : mLED;
        const bool mState = !mLEDs[mID]; 
        led(pLED, mState);
    }

    int digitalRead(uint32_t pPin) {  
        uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
        return mPins[mID];
    }

    void digitalWrite(uint32_t pPin, int8_t pValue) {
        uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
        mPins[mID] = pValue;

        std::vector<float> mData;
        mData.push_back(KLST_OSC_SIM_DIGITAL_WRITE);
        mData.push_back(mID);
        mData.push_back(pValue);
        klangstrom_arduino_sim_transmit(mData);
    }

    void process_receive(const osc::ReceivedMessage &msg) {
        /**
         * typetag has to start with a single integer ( message type `KLST_OSC_SIM_` ) 
         * followed by one or more floats ( date ).
         * 
         * the message type defines what kind of event or function will be triggered.
         */
        if (msg.ArgumentCount() > 0 && str_begins_with("if", msg.TypeTags())) {
            const uint8_t mLength = msg.ArgumentCount() - 1;
            osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
            const uint8_t mMessageType = (uint8_t)arg->AsInt32();
            float mData[mLength];
            uint8_t i = 0;
            ++arg;
            while(arg != msg.ArgumentsEnd()) {
                mData[i] = arg->AsFloat();
                ++arg;
                ++i;
            }
            switch(mMessageType) {
                case KLST_OSC_SIM_LED:
                    if (mLength == 2) {
                        led((uint8_t) mData[0], (uint8_t)mData[1]);
                    }
                    break;
                case KLST_OSC_SIM_DIGITAL_WRITE:
                    if (mLength == 2) {
                        digitalWrite((uint32_t) mData[0], (int8_t)mData[1]);
                    }
                    break;
                case EVENT_ENCODER_ROTATE:
                    if (mLength == 3 && str_equals(msg.TypeTags(), "ifff")) {
                        event_receive(EVENT_ENCODER_ROTATE, mData);
                    } else {
                        emit_message_error("EVENT_ENCODER_ROTATE", "ifff", msg);
                    }
                    break;
                case EVENT_ENCODER_BUTTON_PRESSED:
                    if (mLength == 1 && str_equals(msg.TypeTags(), "if")) {
                        event_receive(EVENT_ENCODER_BUTTON_PRESSED, mData);
                    } else {
                        emit_message_error("EVENT_ENCODER_BUTTON_PRESSED", "if", msg);
                    }
                    break;
                case EVENT_ENCODER_BUTTON_RELEASED:
                    if (mLength == 1 && str_equals(msg.TypeTags(), "if")) {
                        event_receive(EVENT_ENCODER_BUTTON_RELEASED, mData);
                    } else {
                        emit_message_error("EVENT_ENCODER_BUTTON_RELEASED", "if", msg);
                    }
                    break;
                default:
                //@todo(consider to pass *unrecognized* message to a callback function )
                    break;
            }
        }   
    }

    bool* getLEDs() {
        return mLEDs;
    }

private:
    const static uint8_t NUMBER_OF_PINS = NUMBER_OF_GPIO_PINS;
    int8_t mPins[NUMBER_OF_PINS] = { LOW };
    bool mLEDs[NUMBER_OF_LEDS] = { false };

    bool str_begins_with(const char *pre, const char *str) {
        size_t lenpre = strlen(pre),
            lenstr = strlen(str);
        return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
    }
    bool str_equals(const char *pStringA, const char *pStringB) {
        return (strcmp(pStringA, pStringB) == 0);
    }

    void emit_message_error(const char *pMessage, const char *pTypeTag, const osc::ReceivedMessage &msg) {
        KLST_LOG.print("+++ ERROR / ");
        KLST_LOG.print("+++ malformed OSC message: ");
        KLST_LOG.print(pMessage);
        KLST_LOG.print(" : ");
        KLST_LOG.print(pTypeTag);
        KLST_LOG.print(" > ");
        KLST_LOG.print(msg.AddressPattern());
        KLST_LOG.print(" : ");
        KLST_LOG.print(msg.TypeTags());
        KLST_LOG.println();
    }
};
#endif /* KSLT_simulator_h */
