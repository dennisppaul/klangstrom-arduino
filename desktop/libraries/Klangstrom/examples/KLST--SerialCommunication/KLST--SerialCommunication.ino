//
//  KLST--SerialCommunication
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 55;
int mCounter = 0;

void setup() {
    Serial.begin(115200);

    Klang::lock();
    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_frequency(mFreq);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    Serial.println("--");
    SERIAL_00.println("00");
    SERIAL_01.println("01");

    //    while (SERIAL_00.available()) {
    //      int reval = SERIAL_00.read();
    //      Serial.println(">");
    //      Serial.write(reval);
    //      digitalWrite(LED_02, HIGH);
    //    }
    //    while (SERIAL_01.available()) {
    //      int reval = SERIAL_01.read();
    //      Serial.println(">");
    //      Serial.write(reval);
    //      digitalWrite(LED_01, HIGH);
    //    }

    klst::led(LED_00, true);
    mCounter++;
    mCounter %= 3;
    mVCO.set_frequency(mFreq + mFreq * mCounter);
    delay(100);
    klst::led(LED_00, false);
    delay(100);
}

//void serialEvent() {
//  while (Serial.available()) {
//    char c = Serial.read();
//    Serial.print(c);
//    SERIAL_00.print(c);
//    SERIAL_01.print(c);
//  }
//}

void data_receive(const uint8_t sender, uint8_t* data, uint8_t length) {
    Serial.println("data_receive");
    if (sender == KLST_SENDER_SERIAL_00) {
        Serial.println("SERIAL_00");
        klst::led(LED_01, true);
    }
    if (sender == KLST_SENDER_SERIAL_01) {
        Serial.println("SERIAL_01");
        klst::led(LED_02, true);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
