---
layout: libdoc
title: NodeVCFLowPass
permalink: /NodeVCFLowPass/
index: 73
---

          [ NODE_VCF_LOW_PASS      ]       
          +------------------------+       
          |                        |       
    IN00--| SIGNAL          SIGNAL |--OUT00
    IN01--| CUTOFF                 |       
    IN02--| RESONANCE              |       
          |                        |       
          +------------------------+       

this filter node is a low filter with resonance. all inputs are *patchable* i.e they can be controlled by another node ( (V)oltage-(C)ontrolled (F)ilter (`VCF`) ). author/source: [Paul Kellett](http://www.musicdsp.org/archive.php?classid=3#259)

### SET_VALUE

- CUTOFF_F32
- RESONANCE_F32


```c
{% include code/ExampleFilters.ino %}
```

### Source Code

[NodeVCFLowPass.hpp]({{ site.baseurl }}{% link _klang-sources/NodeVCFLowPass.hpp.md %})

