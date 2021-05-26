---
layout: libdoc
title: audioblock
permalink: /klangstrom/audioblock/
index: 102
tag: implement
---

called whenever the underlying audiosystem requires a new block of samples.

```c
void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX);
```
