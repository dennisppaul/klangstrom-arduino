---
layout: libdoc
title: NodeSampler
permalink: /NodeSampler/
index: 62
---

          [ NODE_SAMPLER      ]       
          +-------------------+       
          |                   |       
          |            SIGNAL |--OUT00
          |                   |       
          +-------------------+       

this node can store and repoduce a sample of arbitrary length. @note(this node is only usable in text-based programming. the abstract method `kernel` needs to be implemented.)

### SET_VALUE

- SPEED_F32


```c
{% include code/ExampleSampler.ino %}
```

### Source Code

[NodeSampler.hpp]({{ site.baseurl }}{% link _klang-sources/NodeSampler.hpp.md %})

