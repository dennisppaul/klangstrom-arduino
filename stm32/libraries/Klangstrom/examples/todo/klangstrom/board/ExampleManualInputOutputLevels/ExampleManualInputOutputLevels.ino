//
// ExampleManualInputOutputLevels
//

/*  WM8731 datasheet
    
    "Digital Audio Path Control" (0b0000101)
    BIT 3 "DAC Soft Mute Control (Digital)"
          1 = Enable soft mute
          0 = Disable soft mute

    "Analogue Audio Path Control" (0b0000100)
    BIT 3 "Bypass Switch"
          1 = Enable 
          0 = Disable 
    BIT 4 "DAC Select"
          1 = Select DAC 
          0 = Don’t Select DAC
    BIT 5 "Side Tone Switch"
          1 = Enable 
          0 = Disable 

    "Left Headphone Out" (0b0000010)
    BIT 7 "Left Channel Zero Cross detect"
    BIT 8 "Left to Right Channel Headphone Volume, Mute and Zero Cross Data Load Control"
          1 = Enable 
          0 = Disable 

    "Right Headphone Out" (0b0000011)
    BIT 7 "Right Channel Zero Cross detect"
    BIT 8 "Right to Right Channel Headphone Volume, Mute and Zero Cross Data Load Control"
          1 = Enable 
          0 = Disable 
*/

// #ifdef __cplusplus
// extern "C" {
// #endif
// extern bool WM8731_volume(float n);
// #ifdef __cplusplus
// }
// #endif

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klangstrom;

float       mValue    = 1.0;
const float mValueInc = 0.05 / 4;
bool        mToggle   = false;

void setup() {
    option(KLST_OPTION_HEADPHONE_OUTPUT_VOLUME, 0.89);  // set headphone output to approx -1.0V...1.0V
}

void loop() {
}

void beat(uint32_t pBeat) {
    led_toggle(LED_00);
    mToggle = !mToggle;
    Serial.print("OUTPUT: ");
    Serial.println(mValue);
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            mValue = 0;
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).delta > 0) {
                mValue += mValueInc;
                mValue = fmin(1.0, mValue);
            } else {
                mValue -= mValueInc;
                mValue = fmax(-1.0, mValue);
            }
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        pOutputLeft[i]  = mToggle ? mValue : -mValue; // signal needs to be toggles otherwise audiocodec fades voltage to 0V
        pOutputRight[i] = pOutputLeft[i];
    }
}
