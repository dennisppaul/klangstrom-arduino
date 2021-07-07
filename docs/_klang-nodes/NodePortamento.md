---
layout: libdoc
title: NodePortamento
permalink: /NodePortamento/
index: 60
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

### Source Code

[NodePortamento.hpp]({{ site.baseurl }}{% link _klang-sources/NodePortamento.hpp.md %})

