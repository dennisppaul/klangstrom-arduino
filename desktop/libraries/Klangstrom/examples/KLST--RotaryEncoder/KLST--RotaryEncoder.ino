//
//  KLST--RotartyEncoders
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

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
     * events ( e.g encoder, serial ) are processed right after loop is finished.
     */
    delay(100);
}

void event_receive(const uint8_t event, const float* data) {
    print_encoder_state(event, data);

    /* encoders change the frequency when rotated and change the amplitude when pressed */
    switch (event) {
        case EVENT_ENCODER_BUTTON_00:
        case EVENT_ENCODER_BUTTON_01:
        case EVENT_ENCODER_BUTTON_02:
            if (data[BUTTON_STATE]) {
                mADSR.start();
            } else {
                mADSR.stop();
            }
            //      mVCO.set_amplitude(data[BUTTON_STATE] ? 0.5 : 0.0);
            break;
        case EVENT_ENCODER_ROTATE_00:
        case EVENT_ENCODER_ROTATE_01:
        case EVENT_ENCODER_ROTATE_02:
            float mFreqChange = data[TICK] - data[PREVIOUS_TICK];
            mFreq += mFreqChange;
            mVCO.set_frequency(mFreq);
            break;
    }
}

void print_encoder_state(const uint8_t event, const float* data) {
    /* print encoder states + toggle LEDs*/
    if (event == EVENT_ENCODER_ROTATE_00) {
        Serial.print("ENCODER_00 ROTATION: ");
        Serial.print(data[TICK]);
        Serial.print(", ");
        Serial.print(data[TICK] - data[PREVIOUS_TICK]);
        Serial.println();
        led_toggle(LED_00);
    }
    if (event == EVENT_ENCODER_ROTATE_01) {
        Serial.print("ENCODER_01 ROTATION: ");
        Serial.print(data[TICK]);
        Serial.print(", ");
        Serial.print(data[TICK] - data[PREVIOUS_TICK]);
        Serial.println();
        led_toggle(LED_01);
    }
    if (event == EVENT_ENCODER_ROTATE_02) {
        Serial.print("ENCODER_02 ROTATION: ");
        Serial.print(data[TICK]);
        Serial.print(", ");
        Serial.print(data[TICK] - data[PREVIOUS_TICK]);
        Serial.println();
        led_toggle(LED_02);
    }
    if (event == EVENT_ENCODER_BUTTON_00) {
        Serial.print("ENCODER_00 BUTTON  : ");
        Serial.println((int)data[BUTTON_STATE]);
    }
    if (event == EVENT_ENCODER_BUTTON_01) {
        Serial.print("ENCODER_01 BUTTON  : ");
        Serial.println((int)data[BUTTON_STATE]);
    }
    if (event == EVENT_ENCODER_BUTTON_02) {
        Serial.print("ENCODER_02 BUTTON  : ");
        Serial.println((int)data[BUTTON_STATE]);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
