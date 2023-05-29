/*
 * this example demonstrates how to generate speech output from text using the famous Software Automatic Mouth ( SAM )
 * engine. note that `#define KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM` is required to use the reverb effect.
 *
 * allocate buffer for pre-rendered voices use `get_used_buffer_length()` to get text length
 * *after* text is rendered with `speak()`.
 */

#define KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeTextToSpeechSAM mTTS(46160);

void setup() {
    Klang::connect(mTTS, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mTTS.speak("/HAA5RGLOW MAY5N NAA5RMEY4 IHSD \x9b\x9b\0", true);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_KEYBOARD:
            handle_key_pressed(keyboard_event(data).keys[0]);
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_event(data).index == ENCODER_00) {
                mTTS.set_speed(100);
                mTTS.set_pitch(96);
                mTTS.speak("KLAH5NXSHDROW5M \x9b\x9b\0", true);
            } else if (encoder_event(data).index == ENCODER_01) {
                mTTS.set_speed(72);
                mTTS.set_pitch(64);
                mTTS.speak("hello my name is");
            } else if (encoder_event(data).index == ENCODER_02) {
                mTTS.set_speed(100);
                mTTS.set_pitch(96);
                mTTS.speak("klangstrom");
            }
            break;
    }
}

void handle_key_pressed(int key) {
    mTTS.speak_ascii(key);
}
