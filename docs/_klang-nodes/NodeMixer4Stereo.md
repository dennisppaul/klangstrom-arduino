---
layout: libdoc
title: NodeMixer4Stereo
permalink: /NodeMixer4Stereo/
index: 53
---

          [ NODE_MIXER4_STEREO      ]       
          +-------------------------+       
          |                         |       
    IN00--| SIGNAL_0    SIGNAL_LEFT |--OUT00
    IN01--| SIGNAL_1   SIGNAL_RIGHT |--OUT01
    IN02--| SIGNAL_2                |       
    IN03--| SIGNAL_3                |       
          |                         |       
          +-------------------------+       

this nodes mixes 4 incoming signal into 2. it also allows to define the mix value and the panning for each incoming signal.

### SET_VALUE

- MIX_I8_F32
- PAN_I8_F32


```c
{% include code/ExampleMixer4Stereo.ino %}
```

### Source Code

[NodeMixer4Stereo.hpp]({{ site.baseurl }}{% link _klang-sources/NodeMixer4Stereo.hpp.md %})

