---
layout: libdoc
title: NODE_VCF_MOOG_LP
permalink: /node_vcf_moog_lp/
index: 62
---

          [ NODE_VCF_MOOG_LP      ]       
          +-----------------------+       
          |                       |       
    IN00--| SIGNAL         SIGNAL |--OUT00
    IN01--| CUTOFF                |       
    IN02--| RESONANCE             |       
          |                       |       
          +-----------------------+       

this filter node is a low filter with resonance. all inputs are *patchable* i.e they can be controlled by another node ( (V)oltage-(C)ontrolled (F)ilter (`VCF`) ). author/source: [Stilson/Smith](https://www.musicdsp.org/en/latest/Filters/26-moog-vcf-variation-2.html)

### SET_VALUE

- CUTOFF_F32
- RESONANCE_F32


