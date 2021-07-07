---
layout: libdoc
title: NodeVCOWavetable
permalink: /NodeVCOWavetable/
index: 70
---

          [ NODE_VCO_WAVETABLE      ]       
          +-------------------------+       
          |                         |       
    IN00--| FREQ             SIGNAL |--OUT00
    IN01--| AMP                     |       
          |                         |       
          +-------------------------+       

the wavetable node produces a periodic signal from a chunk of memory ( wavetable ). the speed at which the signal is produced from the wavetable can be adjusted by the changing its frequency `FREQ` and the amplitude by changing `AMP`. the node is often used as an *oscillator* to produce a sinewave, triangle, square or sawtooth shaped signal at an audible frequency ( (V)oltage-(C)ontrolled (O)scillator (`VCO`) ). however, this node can also be used to manipulate other parameters ( e.g the frequency of another node ) at a lower frequency ( (L)ow (F)requency (O)scillator (`LFO`) ).

### SET_VALUE

- FREQUENCY_F32
- AMPLITUDE_F32
- OFFSET_F32
- WAVEFORM_I8


```c
{% include code/ExampleOscillators.ino %}
```

### Source Code

[NodeVCOWavetable.hpp]({{ site.baseurl }}{% link _klang-sources/NodeVCOWavetable.hpp.md %})

