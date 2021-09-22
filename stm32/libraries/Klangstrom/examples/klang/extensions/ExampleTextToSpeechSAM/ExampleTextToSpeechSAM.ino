//
//  ExampleTextToSpeechSAM
//

#define KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC mDAC;
/*
 * allocate buffer for pre-rendered voices use `get_used_buffer_length()` to get text length
 * *after* text is rendered with `speak()`.
 */
NodeTextToSpeechSAM mTTS(46160);

void setup() {
    Klang::connect(mTTS, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    mTTS.speak("/HAA5RGLOW MAY5N NAA5RMEY4 IHSD \x9b\x9b\0", true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(data[KEY]);
            break;
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (data[INDEX] == ENCODER_00) {
                mTTS.set_speed(100);
                mTTS.set_pitch(96);
                mTTS.speak("KLAH5NXSHDROW5M \x9b\x9b\0", true);
                break;
            } else if (data[INDEX] == ENCODER_01) {
                mTTS.set_speed(72);
                mTTS.set_pitch(64);
                mTTS.speak("hello my name is");
                break;
            } else if (data[INDEX] == ENCODER_02) {
                mTTS.set_speed(100);
                mTTS.set_pitch(96);
                mTTS.speak("klangstrom");
                break;
            }
    }
}

void handle_key_pressed(int key) {
    mTTS.speak_ascii(key);
}
