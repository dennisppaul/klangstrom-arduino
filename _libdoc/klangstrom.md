---
layout: libdoc
title: Klangstrom ( Application )
permalink: /klangstrom/
index: 10
---

*Klangstrom* supplies an application structure to facilitate the development of applications. 

*Klangstrom* extends the arduino *idiom* ( i.e `setup`) + `loop()` ) with sound, music + event related functions ( e.g `audioblock()` + `beat()` ). *Klangstrom* also implements a simple abstraction layer to unify access to peripherals ( e.g `data_receive()` + `data_transmit()` ).

the following functions can ( but do not have to be ) implemented in the application:

<ul>
{% assign items = site.klangstrom | sort: 'index' %}
{% for page in items %}
    {% if page.tag == "implement" %}
    <li><code><a href="{{ page.url | relative_url }}">{{ page.title }}()</a></code> {{ page.excerpt | strip_html }}</li>
    {% endif %}
{% endfor %}
</ul>

the following functions allow to configure and query application and hardware states or communicate with peripherals ( e.g LEDs, buttons, GPIOs, UART(serial), SPI, I2C, SD Card ):

<ul>
{% assign items = site.klangstrom | sort: 'index' %}
{% for page in items %}
    {% if page.tag == "library" %}
    <li>
    <code><a href="{{ page.url | relative_url }}">{{ page.title }}()</a></code> {{ page.excerpt | strip_html }}
    </li>
    {% endif %}
{% endfor %}
</ul>

## Blink: An Example Application

in this examples a tone is played and changed periodically while an LED is turned on and off. in the `setup()` function two nodes are connected and the `beat` function is configured to be called once a second. the `loop()` function is implemented but does not contain any *real* functionality. in the `beat()` function LED and tone are changed every odd beat to one state and every even beat to another. in `audioblock()` the left and right output buffers are populated by the DAC node.

```c
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

void setup() {
    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
    beats_per_minute(60);
}

void loop() {
    delay(10);                    // wait for 0.01 sec
}

void beat(uint32_t pBeat) {
    if ( (pBeat%2)==0 ) {
        led(LED_00, true);        // turn LED_00 on ( `true` is ON )
        mVCO.set_amplitude(0.25); // set amplitude to 25%
        mVCO.set_frequency(110);  // set frequencey to 110Hz
    } else {
        led(LED_00, false);       // turn LED_00 off ( `false` is OFF )
        mVCO.set_amplitude(0.0);  // set amplitude to 0%
        mVCO.set_frequency(200);  // set frequencey to 220Hz
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft,  SIGNAL_TYPE* pInputRight) {
    /* process next audio block */
    mDAC.process_frame(pOutputLeft, pOutputRight); 
}
```

see [Klang]({{ site.baseurl }}{% link _libdoc/klang.md %}) documentation for further details on nodes.
