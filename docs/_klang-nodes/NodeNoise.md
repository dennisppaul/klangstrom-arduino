---
layout: libdoc
title: NodeNoise
permalink: /NodeNoise/
index: 60
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

### Source Code

[NodeNoise.hpp]({{ site.baseurl }}{% link _klang-sources/NodeNoise.hpp.md %})

