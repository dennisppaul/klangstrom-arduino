//
//  ExampleChorus
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable    mOsc;
NodeChorus          mChorus;
NodeDAC             mDAC;

void setup()  {
    Klang::connect(mOsc,      mChorus);
    Klang::connect(mChorus,   NodeChorus::CH_OUT_SIGNAL_LEFT,     mDAC,   NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mChorus,   NodeChorus::CH_OUT_SIGNAL_RIGHT,    mDAC,   NodeDAC::CH_IN_SIGNAL_RIGHT);

    mChorus.set_stereo(true);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);

    mDAC.set_stereo(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(data[KEY]);
            break;
        case EVENT_MOUSE_MOVED:
            mChorus.set_feedback(data[X]);
            mChorus.set_sweep(data[Y]);
            break;
        case EVENT_MOUSE_DRAGGED:
            mChorus.set_rate(data[X]);
            mChorus.set_delay(data[Y]);
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mChorus.set_mode(true);
            break;
        case '2':
            mChorus.set_mode(false);
            break;
        case '3':
            mChorus.set_feedback_sign(true);
            break;
        case '4':
            mChorus.set_feedback_sign(false);
            break;
    }
}
