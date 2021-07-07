---
layout: libdoc
title: NodeMixer4
permalink: /NodeMixer4/
index: 47
---

          [ NODE_MIXER4       ]       
          +-------------------+       
          |                   |       
    IN00--| SIGNAL_0   SIGNAL |--OUT00
    IN01--| SIGNAL_1          |       
    IN02--| SIGNAL_2          |       
    IN03--| SIGNAL_3          |       
          |                   |       
          +-------------------+       

this nodes mixes 4 incoming signal into 1. it also allows to define the mix value for each incoming signal.

### SET_VALUE

- MIX_I8_F32


```c
{% include code/ExampleMixer4.ino %}
```

### Source Code

[NodeMixer4.hpp]({{ site.baseurl }}{% link _klang-sources/NodeMixer4.hpp.md %})

