---
layout: libdoc
title: NodeDAC
permalink: /NodeDAC/
index: 40
---

          [ NODE_DAC         ]
          +------------------+
          |                  |
    IN00--| SIGNAL(_LEFT)    |
    IN01--| SIGNAL_RIGHT     |
          |                  |
          +------------------+

the DAC ( (D)igital (A)nalog (C)onverter ) node represents a connection to the underlying hardware sound output. oftentimes it is the last node in a synthesizer patch. it accepts mono or stereo inputs.

### SET_VALUE

- STEREO_I8


### Source Code

[NodeDAC.hpp]({{ site.baseurl }}{% link _klang-sources/NodeDAC.hpp.md %})

