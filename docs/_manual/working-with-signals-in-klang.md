---
layout: page
title: Working with Signals in Klang
permalink: /working-with-signals-in-klang/
index: 31
---

with *Klang* signal networks can be formed that create data streams which eventually become audible sound. the data that *flows* through the nodes is called *signals*.

## Signals

most nodes in *Klang* produce audio signals. however, some nodes can also be used to produce signals that control other aspects like parameters or events. but let us have a look at audio signals first:

### Audio Signals

in *Klang* audio signals are expressed in floating-point numbers and range from `-1.0` to `1.0`. an audio signal can be thought of as a stream of floating-point numbers where each number itself is also refered to as a *sample*.

in *Klangstrom* a neuralgic point for audio signals is the function [`audioblock()`]({{ site.baseurl }}{% link _klangstrom/audioblock.md %}) which is implemented in an application and is perpetually called by the underlying audio hardware. every time it is called the underlying hardware requests new sample data. but instead of requesting just single samples ( or frames ) it requests blocks of sample data. the size of each block is defined in the constant `KLANG_SAMPLES_PER_AUDIO_BLOCK`. note that the function uses `SIGNAL_TYPE` as its parameter data type. `SIGNAL_TYPE` is defined as a 32BIT `float` type.

the following application connects an oscillator node ( `mVCO` ) to an output node ( `mDAC` ). the oscillator produces a sinewave-shaped signal at a frequency of 110Hz with an amplitude of 25% of the maximum volume ( i.e oscillating between values ranging from `-0.25` to `0.25` ). in `audioblock()` the output node collects the sample data from the oscillator and writes it into the output buffers. this application produces a constant tone.

```c
#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

void setup() {
    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft,  SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight); 
}
```

by default the KLST_TINY board runs at a sampling rate of 48000Hz and requests audioblocks of 128samples per block. therefore the `audioblock()` function is called 375 per second ( = 48000Hz / 128samples ).

### Parameter Signals

in the application above the oscillator node was used to create an audio signal. as mentioned earlier nodes can also be used to produce signals that control other aspects like parameters or events. let us have a look at how the same type of oscillator can be used to controll the frequency of another oscillator node.

in this application, similar to the application above, an oscillator node ( `mVCO` ) is connected to an ouput node set to produce a signal with an amplitude of 25%. only this time the frequence is not set to a static value but rather controlled by another oscillator ( `mLFO` ) oscillating at a lower frequency of 10Hz ( refered to as *Low Frequency Oscillator* (LFO) ) around the value `110` with an amplitude of 5 ( i.e oscillating between `105.0` and `115.0` ). this application will create a constant tone with a vibrato effect.

```c
#include "Nodes.hpp"

using namespace klang;

NodeDAC             mDAC;
NodeVCOWavetable    mVCO;
NodeVCOWavetable    mLFOFrequency;

void setup()  {
    Klang::connect(mLFOFrequency, Node::CH_OUT_SIGNAL, 
                   mVCO,          NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mVCO,          Node::CH_OUT_SIGNAL, 
                   mDAC,          NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mVCO.set_amplitude(0.25);

    mLFOFrequency.set_frequency(10);
    mLFOFrequency.set_amplitude(5);
    mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOFrequency.set_offset(110);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft,  SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
```

any output signal of one node can be connected to the input of another node.