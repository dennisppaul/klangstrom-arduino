---
layout: libdoc
title: Klang ( Sound + Digital Signal Processing )
permalink: /klang-sound-dsp/
index: 1
---

@todo(this page needs improvement!)

*Klang* is the sound and *digital audio signal processing* (DSP) library of klangstrom or put in other words: *Klang* is a node+text-based synthesizer library.

although *klang* is *text-based* it uses a *node-based model*. *nodes* are objects with inputs and outputs that are connected to form signal networks called *patch*. *klang* comes with a set of *nodes* but can also be extended with custom nodes.

below is a simple patch that connects the output 3 wavetable oscillators to the inputs of a low-pass filter which is connected to an ADSR envelope that is connected to a DAC output:

```
          [ NODE_WAVETABLE ]                                                                                                        
          +----------------+                                                                                                        
          |                |                                                                                                        
    IN00--| FREQ    SIGNAL |--OUT00 >-+                                                                                             
    IN01--| AMP            |          |                                                                                             
          |                |          |                                                                                             
          +----------------+          |                                                                                             
                                      |         [ NODE_FILTER_MOOG ]                  [ NODE_ADSR       ]                  [ NODE_DAC        ]
          [ NODE_WAVETABLE ]          |         +------------------+                  +-----------------+                  +-----------------+ 
          +----------------+          |         |                  |                  |                 |                  |                 |
          |                |          +-> IN00--| SIGNAL    SIGNAL |--OUT00 >-> IN00--| SIGNAL   SIGNAL |--OUT00 >-> IN00--| SIGNAL(LEFT)_IN |
    IN00--| FREQ    SIGNAL |--OUT00 >---> IN01--| CUTOFF           |                  |                 |            IN01--| SIGNAL_IN       |
    IN01--| AMP            |          +-> IN02--| RESONANCE        |                  +-----------------+                  |                 |
          |                |          |         |                  |                                                       +-----------------+
          +----------------+          |         +------------------+                                                                
                                      |                                                                                             
          [ NODE_WAVETABLE ]          |                                                                                             
          +----------------+          |                                                                                             
          |                |          |                                                                                             
    IN00--| FREQ    SIGNAL |--OUT00 >-+                                                                                             
    IN01--| AMP            |                                                                                                        
          |                |                                                                                                        
          +----------------+                                                                                                        
```

@schematic(patch with VCO, LFOs, filter, envelope and output )

below is a translating of the above schematic into source code ( c++ ):

```c
#include "Nodes.hpp"

using namespace klang;

NodeVCFMoogLP mFilter00;
NodeVCOWavetable mOsc01;
NodeVCOWavetable mOsc02;
NodeVCOWavetable mOsc03;
NodeADSR mADSR04;
NodeDAC mDAC05;

void setup() {
    Klang::connect(mOsc01, 0, mFilter00, 0);
    Klang::connect(mOsc02, 0, mFilter00, 1);
    Klang::connect(mOsc03, 0, mFilter00, 2);
    Klang::connect(mFilter00, 0, mADSR04, 0);
    Klang::connect(mADSR04, 0, mDAC05, 0);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, 
                SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft, 
                SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
```
@sourcecode(same patch as above written as source code)
