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
NodeADSR        mADSR;

float mFreq = 55;

void setup() {
  Serial.begin(115200);

  Klang::lock();

  Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mADSR,    NodeADSR::CH_IN_SIGNAL);
  Klang::connect(mADSR,   Node::CH_OUT_SIGNAL, mDAC,     NodeDAC::CH_IN_SIGNAL_LEFT);

  mVCO.set_frequency(mFreq);
  mVCO.set_amplitude(0.5);
  mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

  Klang::unlock();
}

void loop() {
  /* note that the iteration time for each loop needs to be kept short because all
     events ( e.g encoder, serial ) are processed right after loop is finished.
  */
  delay(100);
}

void event_receive(const uint8_t event, const float* data) {
  /* print encoder states + toggle LEDs*/
  if (event == KLST_EVENT_ENCODER_00) {
    Serial.print("ENCODER_00 ROTATION: ");
    Serial.print(data[KLST_EVENT_DATA_TICK]);
    Serial.print(", ");
    Serial.print(data[KLST_EVENT_DATA_TICK] - data[KLST_EVENT_DATA_PREVIOUS_TICK]);
    Serial.println();
    klst::led_toggle(LED_00);
  }
  if (event == KLST_EVENT_ENCODER_01) {
    Serial.print("ENCODER_01 ROTATION: ");
    Serial.print(data[KLST_EVENT_DATA_TICK]);
    Serial.print(", ");
    Serial.print(data[KLST_EVENT_DATA_TICK] - data[KLST_EVENT_DATA_PREVIOUS_TICK]);
    Serial.println();
    klst::led_toggle(LED_01);
  }
  if (event == KLST_EVENT_ENCODER_02) {
    Serial.print("ENCODER_02 ROTATION: ");
    Serial.print(data[KLST_EVENT_DATA_TICK]);
    Serial.print(", ");
    Serial.print(data[KLST_EVENT_DATA_TICK] - data[KLST_EVENT_DATA_PREVIOUS_TICK]);
    Serial.println();
    klst::led_toggle(LED_02);
  }
  if (event == KLST_EVENT_ENCODER_BUTTON_00) {
    Serial.print("ENCODER_00 BUTTON  : ");
    Serial.println((int)data[KLST_EVENT_DATA_BUTTON_STATE]);
  }
  if (event == KLST_EVENT_ENCODER_BUTTON_01) {
    Serial.print("ENCODER_01 BUTTON  : ");
    Serial.println((int)data[KLST_EVENT_DATA_BUTTON_STATE]);
  }
  if (event == KLST_EVENT_ENCODER_BUTTON_02) {
    Serial.print("ENCODER_02 BUTTON  : ");
    Serial.println((int)data[KLST_EVENT_DATA_BUTTON_STATE]);
  }

  /* encoders change the frequency when rotated and change the amplitude when pressed */
  switch (event) {
    case KLST_EVENT_ENCODER_BUTTON_00:
    case KLST_EVENT_ENCODER_BUTTON_01:
    case KLST_EVENT_ENCODER_BUTTON_02:
      if (data[KLST_EVENT_DATA_BUTTON_STATE]) {
        mADSR.start();
      } else {
        mADSR.stop();
      }
      //      mVCO.set_amplitude(data[KLST_EVENT_DATA_BUTTON_STATE] ? 0.5 : 0.0);
      break;
    case KLST_EVENT_ENCODER_00:
    case KLST_EVENT_ENCODER_01:
    case KLST_EVENT_ENCODER_02:
      float mFreqChange = data[KLST_EVENT_DATA_TICK] - data[KLST_EVENT_DATA_PREVIOUS_TICK];
      mFreq += mFreqChange;
      mVCO.set_frequency(mFreq);
      break;
  }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
  mDAC.process_frame(pOutputLeft, pOutputRight);
}
