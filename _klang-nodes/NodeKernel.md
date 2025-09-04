---
layout: libdoc
title: NodeKernel
permalink: /NodeKernel/
index: 45
---

          [ NODE_KERNEL      ]       
          +------------------+       
          |                  |       
    IN00--| SIGNAL    SIGNAL |--OUT00
          |                  |       
          +------------------+       

this node facilitate a simple mechanism to write a *kernel* to process the incoming audio signal per sample. @note(this node is only usable in text-based programming. the abstract method `kernel` needs to be implemented.)


```c
{% include code/ExampleKernel.ino %}
```

### Source Code

[NodeKernel.hpp]({{ site.baseurl }}{% link _klang-sources/NodeKernel.hpp.md %})

