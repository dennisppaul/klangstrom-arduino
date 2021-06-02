//
// ExampleMeasuringPerformance
//

#include "CycleCounter.h"
#include "Nodes.hpp"

// 65.50–65.54μs    :: VCO
// 167.35–167.47μs  :: VCO+VCF
// @todo(add VCO+VCO+VCF)

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mVCO;
NodeVCFMoogLP       mVCF;

float mAudioblockDuration = 0;
uint8_t mBlocksUsed       = 0;

void setup()  {
    Serial.begin(115200);
    while (!Serial);
    Serial.print("MCU clock speed (MHz) ................. : ");
    Serial.println(SystemCoreClock);
    Serial.print("maximum duration of audioblock (μs) ... : ");
    Serial.println(1000000 / (KLANG_AUDIO_RATE / KLANG_SAMPLES_PER_AUDIO_BLOCK));
    Serial.println("---");

    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mVCF, NodeDAC::CH_IN_SIGNAL);
    Klang::connect(mVCF, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mVCO.set_frequency(110);
    mVCO.set_amplitude(0.1);
    mVCO.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);

    klst_enable_cycle_counter();
}

void beat(uint32_t pBeat) {
    klangstrom::led_toggle(LED_00);
    mVCO.set_frequency(55 * random(1, 8));

    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
    Serial.print("number of audioblock used ............. : ");
    Serial.println(mBlocksUsed);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
}
