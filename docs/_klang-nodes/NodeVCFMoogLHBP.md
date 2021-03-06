---
layout: libdoc
title: NodeVCFMoogLHBP
permalink: /NodeVCFMoogLHBP/
index: 74
---

          [ NODE_VCF_MOOG_LHBP      ]       
          +-------------------------+       
          |                         |       
    IN00--| SIGNAL           SIGNAL |--OUT00
    IN01--| CUTOFF                  |       
    IN02--| RESONANCE               |       
          |                         |       
          +-------------------------+       

this filter node is a high, low, and bandpass filter with resonance. all inputs are *patchable* i.e they can be controlled by another node ( (V)oltage-(C)ontrolled (F)ilter (`VCF`) ). author/source: [Paul Kellett: Moog VCF](https://www.musicdsp.org/en/latest/Filters/25-moog-vcf-variation-1.html).

### SET_VALUE

- CUTOFF_F32
- RESONANCE_F32
- TYPE_I8


```c
{% include code/ExampleFilters.ino %}
```

### Source Code

[NodeVCFMoogLHBP.hpp]({{ site.baseurl }}{% link _klang-sources/NodeVCFMoogLHBP.hpp.md %})

