---
layout: libdoc
title: NodeADSR
permalink: /NodeADSR/
index: 34
---

          [ NODE_ADSR       ]       
          +-----------------+       
          |                 |       
    IN00--| SIGNAL   SIGNAL |--OUT00
          |                 |       
          +-----------------+       

this node provides a envelope with four different stages: (A)ttack (D)ecay (S)ustain (R)elease (ADSR). it is usually used to shape the amplitude of an *oscillator*.

### SET_VALUE

- ATTACK_F32
- DECAY_F32
- SUSTAIN_F32
- RELEASE_F32
- START
- STOP


```c
{% include code/ExampleADSR.ino %}
```

### Source Code

[NodeADSR.hpp]({{ site.baseurl }}{% link _klang-sources/NodeADSR.hpp.md %})

