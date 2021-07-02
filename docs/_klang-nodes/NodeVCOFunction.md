---
layout: libdoc
title: NodeVCOFunction
permalink: /NodeVCOFunction/
index: 63
---

          [ NODE_VCO_FUNC      ]       
          +--------------------+       
          |                    |       
    IN00--| FREQ        SIGNAL |--OUT00
    IN01--| AMP                |       
          |                    |       
          +--------------------+       

the oscillator node produces a periodic signal from a selectable function. the frequency `FREQ` and the amplitude `AMP` can be adjusted. the node is often used as an *oscillator* to produce a sinewave, triangle, square or sawtooth shaped signal at an audible frequency ( (V)oltage-(C)ontrolled (O)scillator (`VCO`) ). however, this node can also be used to manipulate other parameters ( e.g the frequency of another node ) at a lower frequency ( (L)ow (F)requency (O)scillator (`LFO`) ).

### SET_VALUE

- FREQUENCY_F32
- AMPLITUDE_F32
- OFFSET_F32
- WAVEFORM_I8


```c
{% include code/ExampleOscillators.ino %}
```

