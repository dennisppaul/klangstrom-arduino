---
layout: libdoc
title: NodePhaser
permalink: /NodePhaser/
index: 64
---

          [ NODE_PHASER      ]       
          +------------------+       
          |                  |       
    IN00--| SIGNAL    SIGNAL |--OUT00
          |                  |       
          +------------------+       

this node applies a phaser effect to the incoming signal. author/source: [Xavier Halgand / Dekrispator_v2](https://github.com/MrBlueXav/Dekrispator_v2)

### SET_VALUE

- FEEDBACK_F32
- WET_F32
- SWEEPRATE_F32
- RANGE_MIN_F32
- RANGE_MAX_F32


```c
{% include code/ExamplePhaser.ino %}
```

### Source Code

[NodePhaser.hpp]({{ site.baseurl }}{% link _klang-sources/NodePhaser.hpp.md %})

