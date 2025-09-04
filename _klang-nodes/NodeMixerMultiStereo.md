---
layout: libdoc
title: NodeMixerMultiStereo
permalink: /NodeMixerMultiStereo/
index: 58
---

          [ NODE_MIXER_MULTI_STEREO      ]       
          +------------------------------+       
          |                              |       
    IN00--| SIGNAL           SIGNAL_LEFT |--OUT00
          |                 SIGNAL_RIGHT |--OUT01
          |                              |       
          +------------------------------+       

this nodes mixes incoming signals into 2 channels.


```c
{% include code/ExampleMixerMultiStereo.ino %}
```

### Source Code

[NodeMixerMultiStereo.hpp]({{ site.baseurl }}{% link _klang-sources/NodeMixerMultiStereo.hpp.md %})

