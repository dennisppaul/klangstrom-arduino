//
// ExampleAnalogRead
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCOLeft;
NodeVCOFunction mVCORight;
NodeDAC         mDAC;
NodeMixer2      mMixer;

int mADC_00 = 0;
int mADC_01 = 0;

void setup() {
    begin_serial_debug(true);

    Klang::lock();

    Klang::connect(mVCOLeft, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mVCORight, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mMixer, mDAC);

    mVCOLeft.set_amplitude(0.25);
    mVCOLeft.set_frequency(440);
    mVCOLeft.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
    mVCORight.set_amplitude(0.25);
    mVCORight.set_frequency(220);
    mVCORight.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);

    Klang::unlock();
}

void loop() {
    mADC_00 = analogRead(ADC_00);
    mADC_01 = analogRead(ADC_01);

    float mLeftFreq = 440.0 - ( mADC_00 / 1024.0 ) * 220.0;
    float mRightFreq = 440.0 - ( mADC_01 / 1024.0 ) * 220.0;
    mVCOLeft.set_frequency(mLeftFreq);
    mVCORight.set_frequency(mRightFreq);
}

void beat(uint32_t pBeat) {
    led_toggle(LED_00);
    Serial.println("---");
    Serial.print("ADC_00: ");
    Serial.println(mADC_00);
    Serial.print("ADC_01: ");
    Serial.println(mADC_01);
}


void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
