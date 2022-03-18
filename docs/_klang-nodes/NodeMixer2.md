---
layout: libdoc
title: NodeMixer2
permalink: /NodeMixer2/
index: 51
---

          [ NODE_MIXER2       ]       
          +-------------------+       
          |                   |       
    IN00--| SIGNAL_0   SIGNAL |--OUT00
    IN01--| SIGNAL_1          |       
    IN02--| MIX               |       
          |                   |       
          +-------------------+       

this nodes mixes 2 incoming signal into 1. it also allows to define the mix ratio.

### SET_VALUE

- MIX_F32


```c
{% include code/ExampleMixer2.ino %}
```

### Source Code

[NodeMixer2.hpp]({{ site.baseurl }}{% link _klang-sources/NodeMixer2.hpp.md %})

