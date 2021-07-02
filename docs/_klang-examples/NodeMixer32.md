---
layout: libdoc
title: NodeMixer32
permalink: /NodeMixer32/
index: 51
---

          [ NODE_MIXER32       ]       
          +--------------------+       
          |                    |       
    IN00--| SIGNAL_00   SIGNAL |--OUT00
    IN01--| SIGNAL_01          |       
    IN02--| SIGNAL_02          |       
    IN03--| SIGNAL_03          |       
    IN04--| SIGNAL_04          |       
    IN05--| SIGNAL_05          |       
    IN06--| SIGNAL_06          |       
    IN07--| SIGNAL_07          |       
    IN08--| SIGNAL_08          |       
    IN09--| SIGNAL_09          |       
    IN10--| SIGNAL_0A          |       
    IN11--| SIGNAL_0B          |       
    IN12--| SIGNAL_0C          |       
    IN13--| SIGNAL_0D          |       
    IN14--| SIGNAL_0E          |       
    IN15--| SIGNAL_0F          |       
    IN16--| SIGNAL_10          |       
    IN17--| SIGNAL_11          |       
    IN18--| SIGNAL_12          |       
    IN19--| SIGNAL_13          |       
    IN20--| SIGNAL_14          |       
    IN21--| SIGNAL_15          |       
    IN22--| SIGNAL_16          |       
    IN23--| SIGNAL_17          |       
    IN24--| SIGNAL_18          |       
    IN25--| SIGNAL_19          |       
    IN26--| SIGNAL_1A          |       
    IN27--| SIGNAL_1B          |       
    IN28--| SIGNAL_1C          |       
    IN29--| SIGNAL_1D          |       
    IN30--| SIGNAL_1E          |       
    IN31--| SIGNAL_1F          |       
          |                    |       
          +--------------------+       

this nodes mixes 32 incoming signal into 1. it also allows to define the mix value for each incoming signal.


```c
{% include code/ExampleMixer32.ino %}
```

