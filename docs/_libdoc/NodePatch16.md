---
layout: libdoc
title: NodePatch16
permalink: /NodePatch16/
index: 53
---

          [ NODE_PATCH16        ]       
          +---------------------+       
          |                     |       
    IN00--| SIGNAL_0   SIGNAL_0 |--OUT00
    IN01--| SIGNAL_1   SIGNAL_1 |--OUT01
    IN02--| SIGNAL_2   SIGNAL_2 |--OUT02
    IN03--| SIGNAL_3   SIGNAL_3 |--OUT03
    IN04--| SIGNAL_4   SIGNAL_4 |--OUT04
    IN05--| SIGNAL_5   SIGNAL_5 |--OUT05
    IN06--| SIGNAL_6   SIGNAL_6 |--OUT06
    IN07--| SIGNAL_7   SIGNAL_7 |--OUT07
    IN08--| SIGNAL_8   SIGNAL_8 |--OUT08
    IN09--| SIGNAL_9   SIGNAL_9 |--OUT09
    IN10--| SIGNAL_A   SIGNAL_A |--OUT10
    IN11--| SIGNAL_B   SIGNAL_B |--OUT11
    IN12--| SIGNAL_C   SIGNAL_C |--OUT12
    IN13--| SIGNAL_D   SIGNAL_D |--OUT13
    IN14--| SIGNAL_E   SIGNAL_E |--OUT14
    IN15--| SIGNAL_F   SIGNAL_F |--OUT15
          |                     |       
          +---------------------+       

a patch is a collection of one or more nodes. this node facilitates a structure to encapsulate complex node arrangements in one singal node. this node has 16 signal input and 16 output.


```c
{% include code/ExamplePatch16.ino %}
```

