---
layout: libdoc
title: NodeSplitter
permalink: /NodeSplitter/
index: 63
---

          [ NODE_SPLITTER      ]       
          +--------------------+       
          |                    |       
    IN00--| SIGNAL      SIGNAL |--OUT00
          |                    |       
          +--------------------+       

this node stores an incoming signal block in a buffer for later use. it is one of the few nodes that allows to connect multiply inputs to its output.


```c
{% include code/ExampleSplitter.ino %}
```

### Source Code

[NodeSplitter.hpp]({{ site.baseurl }}{% link _klang-sources/NodeSplitter.hpp.md %})

