//
//  KLST--RotartyEncoders
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Klangstrom.h"
#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 110.0;
bool mLED_00_Toggle = false;
bool mLED_01_Toggle = false;
uint32_t mBeatDuration = 0;
uint32_t mBeatDurationBase= 1000;
uint32_t mBeatDurationMax= 120000;
uint32_t mBeatDurationInc = 1000;

void setup() {
  Serial.begin(115200);

  Klang::lock();

  Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

  mVCO.set_frequency(mFreq);
  mVCO.set_amplitude(0.5);
  mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

  Klang::unlock();

  klst_set_BPM(120);
}

void beat(uint32_t pBeat) {
  mLED_01_Toggle = !mLED_01_Toggle;
  klst_led(LED_02, mLED_01_Toggle);

  mVCO.set_frequency(mFreq * ( mLED_01_Toggle ? 1 : 2 ) - 110 * mBeatDuration / mBeatDurationMax);
  mBeatDuration += mBeatDurationInc;
  if (mBeatDuration > mBeatDurationMax) {
    mBeatDuration=0;
  }
  klst_set_BPM_ms(mBeatDurationBase+mBeatDuration);
}

void loop() {
  mLED_00_Toggle = !mLED_00_Toggle;
  delay(500);
  klst_led(LED_00, mLED_00_Toggle);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
  mDAC.process_frame(pOutputLeft, pOutputRight);
}
