---
layout: libdoc
title: NodeMute
permalink: /NodeMute/
index: 54
---

          [ NODE_MUTE       ]       
          +-----------------+       
          |                 |       
    IN00--| SIGNAL   SIGNAL |--OUT00
          |                 |       
          +-----------------+       

this node allows to mute an entire branch of nodes connected to this its input. the muting of a branch will also result in halting the processing of all connected note ( e.g envelopes not being advanced ). optionally, the incoming audio block can be filled with zeros.

### SET_VALUE

- MUTE_I8


```c
{% include code/ExampleMute.ino %}
```

