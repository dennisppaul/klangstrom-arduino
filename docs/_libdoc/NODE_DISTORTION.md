---
layout: libdoc
title: NODE_DISTORTION
permalink: /node_distortion/
index: 41
---

          [ NODE_DISTORTION      ]       
          +----------------------+       
          |                      |       
    IN00--| SIGNAL        SIGNAL |--OUT00
          |                      |       
          +----------------------+       

this node applies distortion to the incoming signal. the clipping type can be `CLIP`, `FOLDBACK`, or `FOLDBACK_SINGLE`. `CLIP` simply clamps the incoming signal to the specified clip value while `FOLDBACK` + `FOLDBACK_SINGLE` mirrors the signal around the specified clip value.

### SET_VALUE

- CLIP_F32
- AMPLIFICATION_F32
- TYPE_I8

