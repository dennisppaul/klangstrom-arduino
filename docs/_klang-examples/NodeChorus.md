---
layout: libdoc
title: NodeChorus
permalink: /NodeChorus/
index: 37
---

          [ NODE_CHORUS          ]       
          +----------------------+       
          |                      |       
    IN00--| SIGNAL   SIGNAL_LEFT |--OUT00
          |         SIGNAL_RIGHT |--OUT01
          |                      |       
          +----------------------+       

this node applies a chorus effect to the incoming signal. author/source: [Xavier Halgand / Dekrispator_v2](https://github.com/MrBlueXav/Dekrispator_v2)

### SET_VALUE

- STEREO_I8
- FEEDBACK_F32
- DELAY_F32
- RATE_F32
- SWEEP_F32


```c
{% include code/ExampleChorus.ino %}
```

