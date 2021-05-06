//
//  KLST--ExampleTextToSpeechSAM.hpp
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#define KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM

#include "Nodes.hpp"

using namespace klang;

NodeDAC             mDAC;
NodeTextToSpeechSAM mTTS;

void setup() {
    Klang::connect(mTTS, Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
    mTTS.speak("hello my name is");
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
            mTTS.speak("/HAALAOAO MAYN NAAMAEAE IHSTT SAEBAASTTIHAAN \x9b\x9b\0", true);
            break;
    }
}

void handle_key_pressed(int key) {
    mTTS.speak_ascii(key);
}
