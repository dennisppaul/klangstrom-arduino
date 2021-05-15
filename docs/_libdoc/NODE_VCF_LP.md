---
layout: libdoc
title: NODE_VCF_LP
permalink: /node_vcf_lp/
index: 60
---

          [ NODE_VCF_LP      ]       
          +------------------+       
          |                  |       
    IN00--| SIGNAL    SIGNAL |--OUT00
    IN01--| CUTOFF           |       
    IN02--| RESONANCE        |       
          |                  |       
          +------------------+       

this filter node is a low filter with resonance. all inputs are *patchable* i.e they can be controlled by another node ( (V)oltage-(C)ontrolled (F)ilter (`VCF`) ). author/source: [Paul Kellett](http://www.musicdsp.org/archive.php?classid=3#259)

### SET_VALUE

- CUTOFF_F32
- RESONANCE_F32


