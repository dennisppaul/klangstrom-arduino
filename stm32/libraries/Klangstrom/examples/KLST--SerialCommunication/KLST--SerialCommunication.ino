//
//  KLST--SerialCommunication
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 55;
int mCounter = 0;

void setup() {
    Serial.begin(115200);

    Klang::lock();
    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_frequency(mFreq);
    mVCO.set_amplitude(0.25);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    led(LED_00, true);
    mCounter++;
    mCounter %= 3;
    mVCO.set_frequency(mFreq + mFreq * mCounter);

    uint8_t mData[3] = {42, 23, 3};
    data_transmit(KLST_SERIAL_00, mData, 3);

    delay(500);
    led(LED_00, false);
    delay(500);
}

void data_receive(const uint8_t sender, uint8_t* data, uint8_t length) {
    if (sender == KLST_SERIAL_00) {
        Serial.print("SERIAL_00 :: ");
        led(LED_01, true);
        for (int i=0; i<length; i++) {
            Serial.print((int)data[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
    if (sender == KLST_SERIAL_01) {
        Serial.print("SERIAL_01 :: ");
        led(LED_02, true);
        for (int i=0; i<length; i++) {
            Serial.print((int)data[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
