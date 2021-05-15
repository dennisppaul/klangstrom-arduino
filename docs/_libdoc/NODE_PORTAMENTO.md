---
layout: libdoc
title: NODE_PORTAMENTO
permalink: /node_portamento/
index: 55
---

          [ NODE_PORTAMENTO      ]       
          +----------------------+       
          |                      |       
    IN00--| VALUE          VALUE |--OUT00
    IN01--| SPEED                |       
          |                      |       
          +----------------------+       

this node interpolates from one a current value to a desired value over time.

### SET_VALUE

- VALUE_F32
- SPEED_F32


```c
{% include code/ExamplePortamento.ino %}
```

