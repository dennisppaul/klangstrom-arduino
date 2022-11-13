#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont_5x8.h"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction fVCO;
NodeDAC         fDAC;

KlangstromDisplayDrawBuffer fDrawBuffer(32);
KlangstromDisplay*          Display;

void setup() {
    Display = KlangstromDisplay::create();
    Display->begin();
    Display->background(0, 0, 0);
    Display->color(255, 255, 255);
    Display->clear();

    Display->textFont(&Font_5x8);
    Display->text(30, 30, "DRAW AUDIO BUFFER");

    fDrawBuffer.width         = 128;
    fDrawBuffer.height        = 64;
    fDrawBuffer.x             = 30;
    fDrawBuffer.y             = 45;
    fDrawBuffer.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
    fDrawBuffer.draw_baseline = true;
    fDrawBuffer.draw_frame(Display);

    Klang::lock();
    Klang::connect(fVCO, Node::CH_OUT_SIGNAL, fDAC, NodeDAC::CH_IN_SIGNAL);
    fVCO.set_amplitude(0.5);
    fVCO.set_frequency(440.0);
    fVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();

    beats_per_minute(480);
}

void beat(uint32_t pBeat) {
    fDrawBuffer.draw_buffer(Display);
}

void loop() {
    LED(LED_00, LED_ON);      // turn LED_00 on
    fVCO.set_amplitude(0.5);  // set amplitude to 50%
    delay(300);               // wait for a second
    LED(LED_00, LED_OFF);     // turn LED_00 off
    fVCO.set_amplitude(0.0);  // set amplitude to 0%
    delay(300);               // wait for a second
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    fDAC.process_frame(pOutputLeft, pOutputRight);
    fDrawBuffer.update_buffer(pOutputRight, KLANG_SAMPLES_PER_AUDIO_BLOCK);
}
