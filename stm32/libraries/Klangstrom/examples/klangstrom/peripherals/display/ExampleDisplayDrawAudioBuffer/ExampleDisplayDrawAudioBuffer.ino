#include "KlangNodes.hpp"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont_5x8.h"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

KlangstromDisplayDrawBuffer mDrawBuffer(32);

void setup() {
    Display.begin();
    Display.background(0, 0, 0);
    Display.color(255, 255, 255);
    Display.clear();

    Display.textFont(&Font_5x8);
    Display.text(30, 30, "DRAW AUDIO BUFFER");

    mDrawBuffer.width         = 128;
    mDrawBuffer.height        = 64;
    mDrawBuffer.x             = 30;
    mDrawBuffer.y             = 45;
    mDrawBuffer.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
    mDrawBuffer.draw_baseline = true;
    mDrawBuffer.draw_frame(Display);

    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mVCO.set_amplitude(0.5);
    mVCO.set_frequency(440.0);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();

    beats_per_minute(480);
}

void beat(uint32_t pBeat) {
    mDrawBuffer.draw_buffer(Display);
}

void loop() {
    LED(LED_00, LED_ON);      // turn LED_00 on
    mVCO.set_amplitude(0.5);  // set amplitude to 50%
    delay(300);               // wait for a second
    LED(LED_00, LED_OFF);     // turn LED_00 off
    mVCO.set_amplitude(0.0);  // set amplitude to 0%
    delay(300);               // wait for a second
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
    mDrawBuffer.update_buffer(pOutputRight, KLANG_SAMPLES_PER_AUDIO_BLOCK);
}
