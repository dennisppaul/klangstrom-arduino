---
layout: libdoc
title: NODE_NOISE
permalink: /node_noise/
index: 50
---

          [ NODE_NOISE      ]       
          +-----------------+       
          |                 |       
          |          SIGNAL |--OUT00
          |                 |       
          +-----------------+       

this node produces a noise signal. if `SIMPLEX` is selected a stride can be specified. `RANDOM` has no options.

### SET_VALUE

- TYPE_I8
- STRIDE_F32


```c
{% include code/ExampleNoise.ino %}
```

