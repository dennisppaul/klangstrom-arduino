---
layout: libdoc
title: NODE_ENVELOPE
permalink: /node_envelope/
index: 42
---

          [ NODE_ENVELOPE      ]       
          +--------------------+       
          |                    |       
    IN00--| SIGNAL      SIGNAL |--OUT00
          |                    |       
          +--------------------+       

this node allows the variation of a value over time.

### SET_VALUE

- TIME_SCALE_F32
- ADD_STAGE_F32_F32
- CLEAR_STAGES
- START
- STOP


```c
{% include code/ExampleEnvelope.ino %}
```

