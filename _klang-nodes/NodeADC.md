---
layout: libdoc
title: NodeADC
permalink: /NodeADC/
index: 32
---

          [ NODE_ADC      ]       
          +---------------+       
          |               |       
          | SIGNAL(_LEFT) |--OUT00
          |  SIGNAL_RIGHT |--OUT01
          |               |       
          +---------------+       

the ADC ( (A)nalog (D)igital (C)onverter ) node represents a connection to the underlying hardware sound input. it accepts mono or stereo outputs.


```c
{% include code/ExampleADCtoDAC.ino %}
```

### Source Code

[NodeADC.hpp]({{ site.baseurl }}{% link _klang-sources/NodeADC.hpp.md %})

