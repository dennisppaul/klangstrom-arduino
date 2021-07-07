---
layout: libdoc
title: NodeDelay
permalink: /NodeDelay/
index: 40
---

          [ NODE_DELAY      ]       
          +-----------------+       
          |                 |       
    IN00--| SIGNAL   SIGNAL |--OUT00
          |                 |       
          +-----------------+       

this node applies a delay ( + filter ) effect to the incoming signal.

### SET_VALUE

- DURATION_F32
- WET_F32
- FEEDBACK_F32
- FILTER_F32


```c
{% include code/ExampleDelay.ino %}
```

### Source Code

[NodeDelay.hpp]({{ site.baseurl }}{% link _klang-sources/NodeDelay.hpp.md %})

