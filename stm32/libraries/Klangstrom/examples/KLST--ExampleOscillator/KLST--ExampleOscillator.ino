//
//  ExampleADSR.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Klangstrom.h"
#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 55;
int mCounter = 0;

void setup() {
  Klang::lock();

  Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

  mVCO.set_frequency(mFreq);
  mVCO.set_amplitude(0.5);
  mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

  Klang::unlock();

  pinMode(PB5, OUTPUT);
  digitalWrite(PB5, HIGH);
  Serial.begin(115200);
  Serial.println("KLST_TINY");
}

void loop() {
  digitalWrite(PB5, HIGH);
  delay(1000);
  digitalWrite(PB5, LOW);
  delay(1000);
  mCounter++;
  if (mCounter > 10) {
    mCounter = 1;
  }
  mVCO.set_frequency(mFreq * mCounter);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
  mDAC.process_frame(pOutputLeft, pOutputRight);
}
